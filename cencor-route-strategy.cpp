/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014-2016,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cencor-route-strategy.hpp"
#include "algorithm.hpp"
#include "core/logger.hpp"
#include <ns3/simulator.h>
#include <cstdlib>

namespace nfd {
namespace fw {

CencorRouteStrategyBase::CencorRouteStrategyBase(Forwarder& forwarder)
  : Strategy(forwarder)
{
}


void
CencorRouteStrategyBase::afterReceiveInterest(const Face& inFace, const Interest& interest,
                                              const shared_ptr<pit::Entry>& pitEntry)

{

   if (hasPendingOutRecords(*pitEntry)) {
    // not a new Interest, don't forward
    return;
  }

  const fib::Entry& fibEntry = this->lookupFib(*pitEntry);

  const fib::NextHopList& nexthops = fibEntry.getNextHops();
 // shared_ptr<const Interest> interest
  if (interest.getName().at(-2).toUri() == "unau"){
    if (interest.getName().at(-1).toSequenceNumber() == 1 ||interest.getName().at(-1).toSequenceNumber() == 3 
             || interest.getName().at(-1).toSequenceNumber() == 5 || interest.getName().at(-1).toSequenceNumber() == 7 
             || interest.getName().at(-1).toSequenceNumber() == 9 ||interest.getName().at(-1).toSequenceNumber() == 11 
             || interest.getName().at(-1).toSequenceNumber() == 13|| interest.getName().at(-1).toSequenceNumber() == 15 
             || interest.getName().at(-1).toSequenceNumber() == 17|| interest.getName().at(-1).toSequenceNumber() == 19 
             || interest.getName().at(-1).toSequenceNumber() == 21|| interest.getName().at(-1).toSequenceNumber() == 23
             || interest.getName().at(-1).toSequenceNumber() == 25|| interest.getName().at(-1).toSequenceNumber() == 27
             || interest.getName().at(-1).toSequenceNumber() == 29 || interest.getName().at(-1).toSequenceNumber() == 31 
             || interest.getName().at(-1).toSequenceNumber() == 33 || interest.getName().at(-1).toSequenceNumber() == 35
             || interest.getName().at(-1).toSequenceNumber() == 37 ||interest.getName().at(-1).toSequenceNumber() == 39 
             || interest.getName().at(-1).toSequenceNumber() == 41 || interest.getName().at(-1).toSequenceNumber() == 43 
             || interest.getName().at(-1).toSequenceNumber() == 45 ||interest.getName().at(-1).toSequenceNumber() == 47 
             || interest.getName().at(-1).toSequenceNumber() == 49|| interest.getName().at(-1).toSequenceNumber() == 51 
             || interest.getName().at(-1).toSequenceNumber() == 99|| interest.getName().at(-1).toSequenceNumber() == 53 
             || interest.getName().at(-1).toSequenceNumber() == 55|| interest.getName().at(-1).toSequenceNumber() == 57
             || interest.getName().at(-1).toSequenceNumber() == 59|| interest.getName().at(-1).toSequenceNumber() == 61
             || interest.getName().at(-1).toSequenceNumber() == 63 || interest.getName().at(-1).toSequenceNumber() == 65 
             || interest.getName().at(-1).toSequenceNumber() == 67 || interest.getName().at(-1).toSequenceNumber() == 69
             || interest.getName().at(-1).toSequenceNumber() == 71 ||interest.getName().at(-1).toSequenceNumber() == 73 
             || interest.getName().at(-1).toSequenceNumber() == 75|| interest.getName().at(-1).toSequenceNumber() == 77 
             || interest.getName().at(-1).toSequenceNumber() == 79|| interest.getName().at(-1).toSequenceNumber() == 81 
             || interest.getName().at(-1).toSequenceNumber() == 83|| interest.getName().at(-1).toSequenceNumber() == 85
             || interest.getName().at(-1).toSequenceNumber() == 87|| interest.getName().at(-1).toSequenceNumber() == 89
             ||interest.getName().at(-1).toSequenceNumber() == 91 || interest.getName().at(-1).toSequenceNumber() == 93 
             || interest.getName().at(-1).toSequenceNumber() == 95 || interest.getName().at(-1).toSequenceNumber() == 97){
      return;
    }else{
        for (fib::NextHopList::const_iterator it = nexthops.begin(); it != nexthops.end(); ++it) {
        Face& outFace = it->getFace();
          if (!wouldViolateScope(inFace, interest, outFace) && canForwardToLegacy(*pitEntry, outFace)) {
            this->sendInterest(pitEntry, outFace, interest);
            return;
          }
        }
    }
 }else{

  for (fib::NextHopList::const_iterator it = nexthops.begin(); it != nexthops.end(); ++it) {
    Face& outFace = it->getFace();
    if (!wouldViolateScope(inFace, interest, outFace) && canForwardToLegacy(*pitEntry, outFace)) {
      this->sendInterest(pitEntry, outFace, interest);
      return;
    }
  }
}
  this->rejectPendingInterest(pitEntry);
}

NFD_REGISTER_STRATEGY(CencorRouteStrategy);

CencorRouteStrategy::CencorRouteStrategy(Forwarder& forwarder, const Name& name)
  : CencorRouteStrategyBase(forwarder)
{
  ParsedInstanceName parsed = parseInstanceName(name);
  if (!parsed.parameters.empty()) {
    BOOST_THROW_EXCEPTION(std::invalid_argument("BestRouteStrategy does not accept parameters"));
  }
  if (parsed.version && *parsed.version != getStrategyName()[-1].toVersion()) {
    BOOST_THROW_EXCEPTION(std::invalid_argument(
      "BestRouteStrategy does not support version " + to_string(*parsed.version)));
  }
  this->setInstanceName(makeInstanceName(name, getStrategyName()));
}

const Name&
CencorRouteStrategy::getStrategyName()
{
  static Name strategyName("/localhost/nfd/strategy/cencor-route/%FD%01");
  return strategyName;
}

} // namespace fw
} // namespace nfd
