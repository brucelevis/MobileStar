#pragma once

#include <ostream>
#include <vector>
#include <cassert>


//색인화된 우선순위 큐
class IndexedPriorityQLow
{
private:
    
    std::vector<float>&    m_vecKeys;
    
    std::vector<int>       m_Heap;
    
    std::vector<int>       m_invHeap;
    
    int                    m_iSize, m_iMaxSize;
    
    void Swap(int a, int b)
    {
        int temp = m_Heap[a]; m_Heap[a] = m_Heap[b]; m_Heap[b] = temp;
        
        //핸들 역시 바꾼다.
        m_invHeap[m_Heap[a]] = a; m_invHeap[m_Heap[b]] = b;
    }
    
    void ReorderUpwards(int nd)
    {
        //힙이 오름차순 정렬될 때 까지 스왑한다.
        while ( (nd>1) && (m_vecKeys[m_Heap[nd/2]] > m_vecKeys[m_Heap[nd]]) )
        {
            Swap(nd/2, nd);
            
            nd /= 2;
        }
    }
    
    void ReorderDownwards(int nd, int HeapSize)
    {
        //힙이 내림차순 정렬될 때 까지 스왑한다.
        while (2*nd <= HeapSize)
        {
            int child = 2 * nd;
            
            //child를 nd의 두 자식보다 작은 인덱스로 설정한다.
            if ((child < HeapSize) && (m_vecKeys[m_Heap[child]] > m_vecKeys[m_Heap[child+1]]))
            {
                ++child;
            }
            
            //nd가 child보다 크면 스왑한다.
            if (m_vecKeys[m_Heap[nd]] > m_vecKeys[m_Heap[child]])
            {
                Swap(child, nd);
                
                //현재 노드를 트리 밑으로 보낸다.
                nd = child;
            }
            
            else
            {
                break;
            }
        }
    }
    
    
public:
    
    IndexedPriorityQLow(std::vector<float>& keys,
                        int              MaxSize):m_vecKeys(keys),
    m_iMaxSize(MaxSize),
    m_iSize(0)
    {
        m_Heap.assign(MaxSize+1, 0);
        m_invHeap.assign(MaxSize+1, 0);
    }
    
    bool empty()const{return (m_iSize==0);}
    
    
    void insert(const int idx)
    {
        assert (m_iSize+1 <= m_iMaxSize);
        
        ++m_iSize;
        
        m_Heap[m_iSize] = idx;
        
        m_invHeap[idx] = m_iSize;
        
        ReorderUpwards(m_iSize);
    }
    
    
    int Pop()
    {
        Swap(1, m_iSize);
        
        ReorderDownwards(1, m_iSize-1);
        
        return m_Heap[m_iSize--];
    }
    
    void ChangePriority(const int idx)
    {
        ReorderUpwards(m_invHeap[idx]);
    }
};