/*
 *  Copyright (c) 2016, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file includes definitions for performing SHA-256 computations.
 */

#ifndef SHA256_HPP_
#define SHA256_HPP_

#include "openthread-core-config.h"

#include <stdint.h>

#include <mbedtls/sha256.h>
#include <psa/crypto.h>

#include <openthread/crypto.h>
#include <openthread/platform/crypto.h>

#include "common/clearable.hpp"
#include "common/equatable.hpp"
#include "common/type_traits.hpp"

namespace ot {

class Message;

namespace Crypto {

/**
 * @addtogroup core-security
 *
 * @{
 *
 */

/**
 * This class implements SHA-256 computation.
 *
 */
class Sha256
{
public:
    /**
     * This type represents a SHA-256 hash.
     *
     */
    class Hash : public otCryptoSha256Hash, public Clearable<Hash>, public Equatable<Hash>
    {
    public:
        static const uint8_t kSize = OT_CRYPTO_SHA256_HASH_SIZE; ///< SHA-256 hash size (bytes)

        /**
         * This method returns a pointer to a byte array containing the hash value.
         *
         * @returns A pointer to a byte array containing the hash.
         *
         */
        const uint8_t *GetBytes(void) const { return m8; }
    };

    /**
     * Constructor for `Sha256` object.
     *
     */
    Sha256(void);

    /**
     * Destructor for `Sha256` object.
     *
     */
    ~Sha256(void);

    /**
     * This method starts the SHA-256 computation.
     *
     */
    void Start(void);

    /**
     * This method inputs bytes into the SHA-256 computation.
     *
     * @param[in]  aBuf        A pointer to the input buffer.
     * @param[in]  aBufLength  The length of @p aBuf in bytes.
     *
     */
    void Update(const void *aBuf, uint16_t aBufLength);

    /**
     * This method inputs an object (treated as a sequence of bytes) into the SHA-256 computation.
     *
     * @tparam    ObjectType   The object type.
     *
     * @param[in] aObject      A reference to the object.
     *
     */
    template <typename ObjectType> void Update(const ObjectType &aObject)
    {
        static_assert(!TypeTraits::IsPointer<ObjectType>::kValue, "ObjectType must not be a pointer");
        return Update(&aObject, sizeof(ObjectType));
    }

    /**
     * This method inputs the bytes read from a given message into the SHA-256 computation.
     *
     * @param[in] aMessage    The message to read the data from.
     * @param[in] aOffset     The offset into @p aMessage to start to read.
     * @param[in] aLength     The number of bytes to read.
     *
     */
    void Update(const Message &aMessage, uint16_t aOffset, uint16_t aLength);

    /**
     * This method finalizes the hash computation.
     *
     * @param[out]  aHash  A reference to a `Hash` to output the calculated hash.
     *
     */
    void Finish(Hash &aHash);

private:
    union Sha256Context
    {
        psa_hash_operation_t   mOperation;
        mbedtls_sha256_context mContext;
    };

    Sha256Context mContext;
};

/**
 * @}
 *
 */

} // namespace Crypto
} // namespace ot

#endif // SHA256_HPP_
