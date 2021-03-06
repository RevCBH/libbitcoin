#ifndef LIBBITCOIN_SESSION_H
#define LIBBITCOIN_SESSION_H

#include <set>

#include <bitcoin/network/hosts.hpp>
#include <bitcoin/network/handshake.hpp>
#include <bitcoin/network/network.hpp>
#include <bitcoin/network/protocol.hpp>
#include <bitcoin/blockchain/blockchain.hpp>
#include <bitcoin/poller.hpp>
#include <bitcoin/transaction_pool.hpp>

namespace libbitcoin {

struct session_params
{
    handshake& handshake_;
    protocol& protocol_;
    blockchain& blockchain_;
    poller& poller_;
    transaction_pool& transaction_pool_;
};

/**
 * Provides a circular buffer of expiring items.
 *
 * The pumpkin_buffer is used to store transaction inventory hashes
 * from the network to avoid re-requesting (and wasting bandwidth).
 * Typically the network goes mad over new tx hashes and several nodes
 * will notify you at once. This class avoids that.
 *
 * As transactions come at a fairly constant rate, we can cheat and make
 * the items in this structure expire by using a circular buffer that
 * overwrites old entries.
 *
 * Thanks copumpkin.
 */
template <typename Item>
class pumpkin_buffer
{
public:
    pumpkin_buffer(size_t max_size)
      : max_size_(max_size), pointer_(0) {}

    void store(const Item& item)
    {
        // Fill it up
        if (expiry_.size() < max_size_)
        {
            lookup_.insert(item);
            expiry_.push_back(item);
            return;
        }
        // Otherwise we overwrite old entries
        BITCOIN_ASSERT(expiry_.size() == max_size_);
        BITCOIN_ASSERT(pointer_ < expiry_.size());
        // First remove it from the hash lookup set
        const Item& erase_item = expiry_[pointer_];
        size_t number_erased = lookup_.erase(erase_item);
        BITCOIN_ASSERT(number_erased == 1);
        // Insert new item and overwrite it in circular buffer
        lookup_.insert(item);
        expiry_[pointer_] = item;
        // Cycle pointer around
        pointer_++;
        if (pointer_ == expiry_.size())
            pointer_ = 0;
    }

    bool exists(const Item& item)
    {
        return lookup_.find(item) != lookup_.end();
    }

private:
    std::set<Item> lookup_;
    std::vector<Item> expiry_;
    size_t max_size_;
    size_t pointer_;
};

class session
{
public:
    typedef std::function<void (const std::error_code&)> completion_handler;

    session(async_service& service, const session_params& params);
    void start(completion_handler handle_complete);
    void stop(completion_handler handle_complete);

private:
    void new_channel(channel_ptr node);
    void set_start_depth(const std::error_code& ec, size_t fork_point,
        const blockchain::block_list& new_blocks,
        const blockchain::block_list& replaced_blocks);

    void inventory(const std::error_code& ec,
        const message::inventory& packet, channel_ptr node);
    void get_data(const std::error_code& ec,
        const message::get_data& packet, channel_ptr node);
    void get_blocks(const std::error_code& ec,
        const message::get_blocks& packet, channel_ptr node);

    void new_tx_inventory(const hash_digest& tx_hash, channel_ptr node);
    void request_tx_data(bool tx_exists,
        const hash_digest& tx_hash, channel_ptr node);

    io_service::strand strand_;

    handshake& handshake_;
    protocol& protocol_;
    blockchain& chain_;
    poller& poll_;
    transaction_pool& tx_pool_;

    pumpkin_buffer<hash_digest> grabbed_invs_;
};

} // namespace libbitcoin

#endif

