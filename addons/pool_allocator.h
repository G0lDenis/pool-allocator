#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <cstddef>
#include <exception>
#include <vector>

#define CHUNK_PTR_SIZE sizeof(uintptr_t)

namespace pool_allocator {
    typedef unsigned char uchar;

    /**
     * Chunk is pool unit.
     * Contains only pointer on next free chunk.
     */
    class Chunk {
    public:
        Chunk* next;
    };

    /**
     * Pool consists of chunks of the same length.
     * It has head chunk - first free chunk.
     */
    class Pool {
    private:
        Chunk* start;
        Chunk* head;
        size_t chunk_size;
        size_t chunk_free;

    public:
        Pool(void* _pos, size_t _chunk_size, size_t _chunk_number)
            : chunk_size(_chunk_size), chunk_free(_chunk_number) {
            head = start = reinterpret_cast<Chunk*>(_pos);
            Chunk* current = head;
            for (auto i = 0; i < _chunk_number - 1; ++i) {
                current->next = reinterpret_cast<Chunk*>(reinterpret_cast<uchar*>(current) + _chunk_size);
                current = current->next;
            }
            current->next = nullptr;
        }

        Chunk* FillChunk() {
            if (!head)
                return nullptr;
            Chunk* chosen = head;
            head = head->next;
            --chunk_free;

            return chosen;
        }

        void FreeChunk(Chunk* _chunk) {
            if (head)
                _chunk->next = head->next;
            else
                _chunk->next = nullptr;
            head = _chunk;
            ++chunk_free;
        }

        [[nodiscard]] bool HasEmptyChunks() const {
            return chunk_free != 0;
        }

        [[nodiscard]] size_t ChunkSize() const {
            return chunk_size;
        }

        [[nodiscard]] const Chunk* Start() const {
            return start;
        }
    };

    bool ComparePools(Pool* pool_left, Pool* pool_right) {
        return pool_left->ChunkSize() < pool_right->ChunkSize();
    }

    /**
     * Main class in this implementation.
     * Stored as binary tree.\n
     * Meets the requirements for
     * <a href="https://en.cppreference.com/w/cpp/named_req/Allocator">
     * Allocator </a>\n
     * Pools are stored in vector.
     */
    class PoolManager {
    private:
        std::vector<Pool*>& pools;

    public:
        explicit PoolManager(std::vector<Pool*>& _pools)
            : pools(_pools) {
            std::sort(pools.begin(), pools.end(), ComparePools);
        }

        template<typename Tp>
        Tp* allocate(size_t _n) {
            if (pools.empty())
                throw std::bad_alloc{};
            size_t need_size = sizeof(Tp) * _n + CHUNK_PTR_SIZE;
            size_t left = 0;
            size_t right = pools.size();
            while (left < right) {
                size_t middle = left + ((right - left) / 2);
                if (pools[middle]->ChunkSize() < need_size) left = middle + 1;
                else right = middle;
            }
            while (left < pools.size()) {
                if (pools[left]->HasEmptyChunks()) {
                    return reinterpret_cast<Tp*>(reinterpret_cast<uchar*>(pools[left]->FillChunk()) + CHUNK_PTR_SIZE);
                }
                left++;
            }
            throw std::bad_alloc{};
        }

        template<typename Tp>
        void deallocate(Tp* _pos, size_t _n) {
            auto* chunk = reinterpret_cast<Chunk*>(reinterpret_cast<uchar*>(_pos) - CHUNK_PTR_SIZE);
            for (auto* _pool: pools) {
                if (_pool->Start() == chunk) {
                    if (reinterpret_cast<uchar*>(chunk) + CHUNK_PTR_SIZE + _n * sizeof(Tp) <=
                        reinterpret_cast<uchar*>(chunk) + _pool->ChunkSize()) {
                        _pool->FreeChunk(chunk);
                    }
                    break;
                }
            }
        }
    };

    /**
     * Pool allocator is only layer between pool manager
     * and stl containers.
     * Parametrizes with stored values type.
     * Has public manager, shared with other allocators.
     * @tparam Tp Stored value type.
     */
    template<typename Tp>
    class PoolAllocator {
    public:
        typedef Tp value_type;
        typedef Tp* pointer;
        typedef size_t size_type;

    public:
        PoolManager* manager;

    public:
        explicit PoolAllocator(PoolManager& _manager)
            : manager(&_manager) {}

        template<typename U>
        explicit PoolAllocator(const PoolAllocator<U>& _alloc)
            : manager(_alloc.manager) {}

        template<typename U>
        PoolAllocator& operator=(const PoolAllocator<U>& _alloc) {
            manager = _alloc.manager;
        }

        value_type* allocate(size_type _n) {
            return manager->allocate<value_type>(_n);
        }

        void deallocate(pointer _pos, size_type _n) {
            manager->deallocate<value_type>(_pos, _n);
        }

        friend bool operator==(const PoolAllocator& left, const PoolAllocator& right) {
            return left.manager == right.manager;
        }

        friend bool operator!=(const PoolAllocator& left, const PoolAllocator& right) {
            return left.manager != right.manager;
        }
    };
}

#endif //POOL_ALLOCATOR_H
