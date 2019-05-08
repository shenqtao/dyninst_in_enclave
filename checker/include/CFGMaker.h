/*
 * See the dyninst/COPYRIGHT file for copyright information.
 * 
 * We provide the Paradyn Tools (below described as "Paradyn")
 * on an AS IS basis, and do not warrant its validity or performance.
 * We reserve the right to update, modify, or discontinue this
 * software at any time.  We shall have no obligation to supply such
 * updates or modifications or any other form of support to you.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef PATCHAPI_CFGMAKER_H_
#define PATCHAPI_CFGMAKER_H_

#include "PatchCFG.h"

namespace Dyninst {

   namespace ParseAPI {
      class Function;
      class Block;
      class Edge;
   };

namespace PatchAPI {

   class PatchObject;
   class PatchFunction;
   class PatchBlock;
   class PatchEdge;


/* A factory class to make / copy CFG structures.
   We provide default implementations.  */

class PATCHAPI_EXPORT CFGMaker {
  public:
    CFGMaker() {}
    virtual ~CFGMaker() {}

    // Make function
    virtual PatchFunction* makeFunction(ParseAPI::Function*,
                                                        PatchObject*);
    virtual PatchFunction* copyFunction(PatchFunction*,
                                                        PatchObject*);

    // Make block
    virtual PatchBlock* makeBlock(ParseAPI::Block*,
                                                  PatchObject*);
    virtual PatchBlock* copyBlock(PatchBlock*, PatchObject*);

    // Make edge
    virtual PatchEdge* makeEdge(ParseAPI::Edge*, PatchBlock*,
                                PatchBlock*, PatchObject*);
    virtual PatchEdge* copyEdge(PatchEdge*, PatchObject*);
};

}
}

#endif /* PATCHAPI_CFGMAKER_H_ */