lib_LTLIBRARIES = libbitcoin.la
AM_CPPFLAGS = -I$(srcdir)/../include
AM_CXXFLAGS = ${my_CXXFLAGS}
libbitcoin_la_SOURCES = \
    satoshi_serialize.cpp \
    deterministic_wallet.cpp \
	getx_responder.cpp \
	async_service.cpp \
	network/network.cpp  \
	network/handshake.cpp  \
	network/channel.cpp \
	network/hosts.cpp \
	network/protocol.cpp \
	poller.cpp \
	utility/serializer.cpp \
	utility/logger.cpp \
	utility/sha256.cpp \
	address.cpp \
	format.cpp \
	script.cpp \
	utility/ripemd.cpp \
	block.cpp \
    utility/elliptic_curve_key.cpp \
	transaction.cpp \
	error.cpp \
    validate.cpp \
	session.cpp \
	utility/base58.cpp \
	utility/big_number.cpp \
	utility/key_formats.cpp \
	constants.cpp \
	blockchain/organizer.cpp \
	blockchain/blockchain.cpp \
	transaction_pool.cpp

if DO_KYOTO
libbitcoin_la_SOURCES += \
	blockchain/kyoto/kyoto_common.cpp \
	blockchain/kyoto/kyoto_blockchain.cpp
endif

if DO_BDB
libbitcoin_la_SOURCES += \
	blockchain/bdb/data_type.cpp \
	blockchain/bdb/txn_guard.cpp \
	blockchain/bdb/bdb_blockchain.cpp \
	blockchain/bdb/bitcoin.pb.cc \
	blockchain/bdb/bdb_chain_keeper.cpp \
	blockchain/bdb/bdb_organizer.cpp \
	blockchain/bdb/bdb_validate_block.cpp \
	blockchain/bdb/bdb_common.cpp \
	blockchain/bdb/protobuf_wrapper.cpp
endif

if DO_POSTGRES
#libbitcoin_la_SOURCES += \
#	blockchain/postgresql/pq_blockchain.cpp \
#	blockchain/postgresql/postgresql_blockchain.cpp
endif
