/*
 * Copyright (c) 2020, Ali Mohammad Pur <mpfard@serenityos.org>
 * Copyright (c) 2024, stelar7 <dudedbz@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/ByteBuffer.h>
#include <AK/ByteString.h>
#include <LibCrypto/ASN1/DER.h>

namespace Crypto::PK {

template<class ByteBuffer>
ErrorOr<ByteBuffer> wrap_in_private_key_info(ByteBuffer key, Span<int const> algorithm_identifier)
{
    ASN1::Encoder encoder;
    TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
        TRY(encoder.write(0x00u)); // version

        // AlgorithmIdentifier
        TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
            TRY(encoder.write(algorithm_identifier)); // algorithm

            return {};
        }));

        // PrivateKey
        TRY(encoder.write(key));

        return {};
    }));

    return encoder.finish();
}

template<class ByteBuffer, typename Params>
ErrorOr<ByteBuffer> wrap_in_private_key_info(ByteBuffer key, Span<int const> algorithm_identifier, Params params)
{
    ASN1::Encoder encoder;
    TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
        TRY(encoder.write(0x00u)); // version

        // AlgorithmIdentifier
        TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
            TRY(encoder.write(algorithm_identifier)); // algorithm

            TRY(encoder.write(params)); // parameters

            return {};
        }));

        // PrivateKey
        TRY(encoder.write(key));

        return {};
    }));

    return encoder.finish();
}

template<typename ExportableKey>
ErrorOr<ByteBuffer> wrap_in_private_key_info(ExportableKey key, Span<int const> algorithm_identifier)
requires requires(ExportableKey k) {
    k.export_as_der();
}
{
    ASN1::Encoder encoder;
    TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
        TRY(encoder.write(0x00u)); // version

        // AlgorithmIdentifier
        TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
            TRY(encoder.write(algorithm_identifier)); // algorithm

            return {};
        }));

        // PrivateKey
        auto data = TRY(key.export_as_der());
        TRY(encoder.write(data));

        return {};
    }));

    return encoder.finish();
}

template<typename ExportableKey, typename Params>
ErrorOr<ByteBuffer> wrap_in_private_key_info(ExportableKey key, Span<int const> algorithm_identifier, Params params)
requires requires(ExportableKey k) {
    k.export_as_der();
}
{
    ASN1::Encoder encoder;
    TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
        TRY(encoder.write(0x00u)); // version

        // AlgorithmIdentifier
        TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
            TRY(encoder.write(algorithm_identifier)); // algorithm

            TRY(encoder.write(params)); // parameters

            return {};
        }));

        // PrivateKey
        auto data = TRY(key.export_as_der());
        TRY(encoder.write(data));

        return {};
    }));

    return encoder.finish();
}

template<class ByteBuffer>
ErrorOr<ByteBuffer> wrap_in_subject_public_key_info(ByteBuffer key, Span<int const> algorithm_identifier)
{
    ASN1::Encoder encoder;
    TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
        // AlgorithmIdentifier
        TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
            TRY(encoder.write(algorithm_identifier)); // algorithm

            return {};
        }));

        // subjectPublicKey
        auto bitstring = ::Crypto::ASN1::BitStringView(key, 0);
        TRY(encoder.write(bitstring));

        return {};
    }));

    return encoder.finish();
}

template<class ByteBuffer, typename ParamsType>
ErrorOr<ByteBuffer> wrap_in_subject_public_key_info(ByteBuffer key, Span<int const> algorithm_identifier, ParamsType const& params)
{
    ASN1::Encoder encoder;
    TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
        // AlgorithmIdentifier
        TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
            TRY(encoder.write(algorithm_identifier)); // algorithm

            TRY(encoder.write(params)); // parameters

            return {};
        }));

        // subjectPublicKey
        auto bitstring = ::Crypto::ASN1::BitStringView(key, 0);
        TRY(encoder.write(bitstring));

        return {};
    }));

    return encoder.finish();
}

template<typename ExportableKey>
ErrorOr<ByteBuffer> wrap_in_subject_public_key_info(ExportableKey key, Span<int const> algorithm_identifier)
requires requires(ExportableKey k) {
    k.export_as_der();
}
{
    ASN1::Encoder encoder;
    TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
        // AlgorithmIdentifier
        TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
            TRY(encoder.write(algorithm_identifier)); // algorithm

            return {};
        }));

        // subjectPublicKey
        auto data = TRY(key.export_as_der());
        auto bitstring = ::Crypto::ASN1::BitStringView(data, 0);
        TRY(encoder.write(bitstring));

        return {};
    }));

    return encoder.finish();
}

template<typename ExportableKey, typename ParamsType>
ErrorOr<ByteBuffer> wrap_in_subject_public_key_info(ExportableKey key, Span<int const> algorithm_identifier, ParamsType const& params)
requires requires(ExportableKey k) {
    k.export_as_der();
}
{
    ASN1::Encoder encoder;
    TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
        // AlgorithmIdentifier
        TRY(encoder.write_constructed(ASN1::Class::Universal, ASN1::Kind::Sequence, [&]() -> ErrorOr<void> {
            TRY(encoder.write(algorithm_identifier)); // algorithm

            TRY(encoder.write(params)); // parameters

            return {};
        }));

        // subjectPublicKey
        auto data = TRY(key.export_as_der());
        auto bitstring = ::Crypto::ASN1::BitStringView(data, 0);
        TRY(encoder.write(bitstring));

        return {};
    }));

    return encoder.finish();
}

template<typename PrivKeyT, typename PubKeyT>
class PKSystem {
public:
    using PublicKeyType = PubKeyT;
    using PrivateKeyType = PrivKeyT;

    PKSystem(PublicKeyType const& pubkey, PrivateKeyType const& privkey)
        : m_public_key(pubkey)
        , m_private_key(privkey)
    {
    }

    PKSystem() = default;

    virtual ErrorOr<ByteBuffer> encrypt(ReadonlyBytes in) = 0;
    virtual ErrorOr<ByteBuffer> decrypt(ReadonlyBytes in) = 0;

    virtual ErrorOr<bool> verify(ReadonlyBytes message, ReadonlyBytes signature) = 0;
    virtual ErrorOr<ByteBuffer> sign(ReadonlyBytes message) = 0;

    virtual ByteString class_name() const = 0;

    virtual size_t output_size() const = 0;

protected:
    virtual ~PKSystem() = default;

    PublicKeyType m_public_key;
    PrivateKeyType m_private_key;
};

}
