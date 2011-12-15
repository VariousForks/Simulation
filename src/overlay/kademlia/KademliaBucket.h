//
// Copyright (C) 2006 Institut fuer Telematik, Universitaet Karlsruhe (TH)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef __KADEMLIA_BUCKET_H
#define __KADEMLIA_BUCKET_H

#include "KademliaNodeHandle.h"

/**
 * @file KademliaBucket.h
 * @author Sebastian Mies, Ingmar Baumgart, Bernhard Heep
 */

class KademliaBucket : public BaseKeySortedVector< KademliaBucketEntry > {
public:
    KademliaBucket(uint16_t maxSize=0,
                   const Comparator<OverlayKey>* comparator = NULL);

    ~KademliaBucket();

    inline void setLastUsage(simtime_t time) {
        this->lastUsage = time;
    }

    inline simtime_t getLastUsage() const {
        return this->lastUsage;
    }

    KademliaBucketEntry* getOldestNode();

    /*
     * Deletes the oldest node in a bucket, provided it is nonempty.
     * For AKademlia.
     *
     * @return true if deletion successful, false if empty.
     */
    bool deleteOldestNode();

    /*
     * If bucket has more than new_size entries, an entry is deleted
     * and true is returned, otherwise false is returned.
     * For AKademlia.
     *
     * @param new_size is the size the bucket should be reduced to.
     *
     * @return true if entry deleted, else false
     */
    bool deleteOldestNodes(uint32_t new_size);

    /*
     * If bucket is bigger than new_size, deletes oldest entries until it is of
     * size equal to new_size. Changes maxSize to new_size, provided this is not
     * equal to 0.
     * For AKademlia.
     *
     * @param new_size should match AKademlia's current k_temp
     * @param new_flexibility should match AKademlia's bucketFlexibility
     *
     * @return true if removes entry/entries, false otherwise.
     */
    bool downsizeBucket(uint32_t new_size, uint32_t new_flexibility);

    std::list<KademliaBucketEntry> replacementCache;

private:
    simtime_t lastUsage;
};

#endif

