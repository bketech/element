/*
    Arc.h - This file is part of Element
    Copyright (C) 2013  Michael Fisher <mfisher31@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef ELEMENT_ARC_H
#define ELEMENT_ARC_H

struct JUCE_API Arc
{
public:
    
    Arc (uint32 sourceNode, uint32 sourcePort, uint32 destNode, uint32 destPort) noexcept;
    virtual ~Arc() { }
    
    uint32 sourceNode;
    uint32 sourcePort;
    uint32 destNode;
    uint32 destPort;
    
private:
    JUCE_LEAK_DETECTOR (Arc)
};

struct ArcSorter
{
    static inline int
    compareElements (const Arc* const first, const Arc* const second) noexcept
    {
        if (first->sourceNode < second->sourceNode)    return -1;
        if (first->sourceNode > second->sourceNode)    return 1;
        if (first->destNode   < second->destNode)      return -1;
        if (first->destNode   > second->destNode)      return 1;
        if (first->sourcePort < second->sourcePort)    return -1;
        if (first->sourcePort > second->sourcePort)    return 1;
        if (first->destPort   < second->destPort)      return -1;
        if (first->destPort   > second->destPort)      return 1;
        
        return 0;
    }
};

/** Holds a fast lookup table for checking which nodes are inputs to others. */
template<class ArcType>
class ArcTable
{
public:
    explicit ArcTable (const OwnedArray<ArcType>& arcs)
    {
        for (int i = 0; i < arcs.size(); ++i)
        {
            const Arc* const c = arcs.getUnchecked(i);
            
            int index;
            Entry* entry = findEntry (c->destNode, index);
            
            if (entry == nullptr)
            {
                entry = new Entry (c->destNode);
                entries.insert (index, entry);
            }
            
            entry->srcNodes.add (c->sourceNode);
        }
    }
    
    bool isAnInputTo (const uint32 possibleInputId,
                      const uint32 possibleDestinationId) const noexcept
    {
        return isAnInputToRecursive (possibleInputId, possibleDestinationId, entries.size());
    }
    
private:
    //==============================================================================
    struct Entry
    {
        explicit Entry (const uint32 destNode_) noexcept : destNode (destNode_) {}
        
        const uint32 destNode;
        SortedSet<uint32> srcNodes;
        
        JUCE_DECLARE_NON_COPYABLE (Entry)
    };
    
    OwnedArray<Entry> entries;
    
    bool isAnInputToRecursive (const uint32 possibleInputId,
                               const uint32 possibleDestinationId,
                               int recursionCheck) const noexcept
    {
        int index;
        
        if (const Entry* const entry = findEntry (possibleDestinationId, index))
        {
            const SortedSet<uint32>& srcNodes = entry->srcNodes;
            
            if (srcNodes.contains (possibleInputId))
                return true;
            
            if (--recursionCheck >= 0)
            {
                for (int i = 0; i < srcNodes.size(); ++i)
                    if (isAnInputToRecursive (possibleInputId, srcNodes.getUnchecked(i), recursionCheck))
                        return true;
            }
        }
        
        return false;
    }
    
    Entry* findEntry (const uint32 destNode, int& insertIndex) const noexcept
    {
        Entry* result = nullptr;
        
        int start = 0;
        int end = entries.size();
        
        for (;;)
        {
            if (start >= end)
            {
                break;
            }
            else if (destNode == entries.getUnchecked (start)->destNode)
            {
                result = entries.getUnchecked (start);
                break;
            }
            else
            {
                const int halfway = (start + end) / 2;
                
                if (halfway == start)
                {
                    if (destNode >= entries.getUnchecked (halfway)->destNode)
                        ++start;
                    
                    break;
                }
                else if (destNode >= entries.getUnchecked (halfway)->destNode)
                    start = halfway;
                else
                    end = halfway;
            }
        }
        
        insertIndex = start;
        return result;
    }
    
    JUCE_DECLARE_NON_COPYABLE (ArcTable)
};

#endif // ELEMENT_ARC_H
