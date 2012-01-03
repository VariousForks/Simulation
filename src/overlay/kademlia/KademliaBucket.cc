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

/**
 * @file KademliaBucket.cc
 * @author Sebastian Mies, Ingmar Baumgart, Bernhard Heep
 */


#include "KademliaBucket.h"

KademliaBucket::KademliaBucket(uint16_t maxSize,
                               const Comparator<OverlayKey>* comparator)
    : BaseKeySortedVector< KademliaBucketEntry >(maxSize, comparator)
{
    lastUsage = -1;
}

KademliaBucket::~KademliaBucket()
{
}

KademliaBucketEntry* KademliaBucket::getOldestNode()
{
	if (this->isEmpty())
		return NULL;

	uint32_t oldest = 0;
	for (uint32_t i = 1;i < this->size();i++) {
		if (this->at(i).getLastSeen() < this->at(oldest).getLastSeen())
			oldest = i;
	}

	return &this->at(oldest);
}

bool KademliaBucket::deleteOldestNode()
{
	if (this->isEmpty()) {
		EV << "\t KademliaBucket::deleteOldestNode(): bucket empty" << endl;
		return false;
	} else {
		uint32_t oldest = 0;
		for (uint32_t i = 1; i < this->size();i++) {
			if (this->at(i).getLastSeen() < this->at(oldest).getLastSeen())
				oldest = i;
		}
		this->erase(this->begin()+oldest);
		return true;
	}
}

bool KademliaBucket::deleteOldestNodes(uint32_t new_size)
{
	if (this->isEmpty()) {
		EV << "\t KademliaBucket::deleteOldestNodes(): bucket empty" << endl;
		return false;
	} else if (this->size() <= new_size) {
		EV << "\t KademliaBucket::deleteOldestNodes(): bucket already small" << endl;
		return false;
	} else {
		uint32_t oldest;
		int count = 0;
		while (this->size() > new_size){
			oldest = 0;
			for (uint32_t i = 1; i < this->size();i++) {
				if (this->at(i).getLastSeen() < this->at(oldest).getLastSeen())
					oldest = i;
			}
			this->erase(this->begin()+oldest);
			++count;
		}
		EV << "\t KademliaBucket::deleteOldestNodes(): " << count
				<< " elements removed" << endl;
		return true;
	}
}

bool KademliaBucket::downsizeBucket(uint32_t new_size, uint32_t new_flexibility)
{
	if (this == NULL){
		EV << "\t KademliaBucket::downsizeBucket(): This bucket is NULL!"
				<< endl;
		return false;
	}
	if (new_size == 0){
		EV << "\t KademliaBucket::downsizeBucket(): New size is 0!"
				<< endl;
		return false;
	} else if (this->size() <= new_size + new_flexibility){
		setMaxSize(new_size + new_flexibility);
		EV << "\t KademliaBucket::downsizeBucket(): resized, no deletions"
				<< endl;
		return false;
	} else {
		while (this->size() > new_size + new_flexibility){
			deleteOldestNodes(new_size + new_flexibility);
		}
		EV << "\t KademliaBucket::downsizeBucket(): resized with deletions"
				<< endl;
		setMaxSize(new_size + new_flexibility);
		return true;
	}
}
