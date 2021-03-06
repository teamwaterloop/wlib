/**
 * @file OpenMap.h
 * @brief Hash map implementation.
 *
 * Hash map is implemented using open addressing and linear probing,
 * based on the assumption that it will be used with good knowledge
 * of the needed map size. The Hash Map provides the same basic
 * functionality as std::unordered_map or std::map_type.
 *
 * @author Jeff Niu
 * @date November 1, 2017
 * @bug No known bugs
 */

#ifndef CORE_STL_MAP_H
#define CORE_STL_MAP_H

#include <wlib/stl/Equal.h>
#include <wlib/stl/Hash.h>
#include <wlib/stl/OpenTable.h>
#include <wlib/stl/Pair.h>
#include <wlib/stl/Table.h>
#include <wlib/stl/Tuple.h>
#include <wlib/utility/Utility.h>

namespace wlp {

    /**
     * Hash map implemented using open addressing and linear probing,
     * in the spirit of std::unordered_map.
     *
     * @tparam Key    key type
     * @tparam Val    value type
     * @tparam Hasher hash function
     * @tparam Equals key equality function
     */
    template<typename Key,
            typename Val,
            typename Hasher = hash<Key, uint16_t>,
            typename Equals = equals<Key>>
    class open_map {
    public:
        typedef open_map<Key, Val, Hasher, Equals> map_type;
        typedef open_table<tuple<Key, Val>,
                Key, Val,
                MapGetKey<Key, Val>, MapGetVal<Key, Val>,
                Hasher, Equals
        > table_type;
        typedef typename table_type::iterator iterator;
        typedef typename table_type::const_iterator const_iterator;
        typedef typename table_type::size_type size_type;
        typedef typename table_type::percent_type percent_type;

        typedef Key key_type;
        typedef Val val_type;

    private:
        table_type m_table;

    public:
        explicit open_map(size_type n = 12, percent_type max_load = 75)
                : m_table(n, max_load) {
        }

        open_map(const map_type &) = delete;

        open_map(map_type &&map)
                : m_table(move(map.m_table)) {
        }

        size_type size() const {
            return m_table.size();
        }

        size_type capacity() const {
            return m_table.capacity();
        }

        percent_type max_load() const {
            return m_table.max_load();
        }

        bool empty() const {
            return m_table.empty();
        }

        const table_type *get_backing_table() const {
            return &m_table;
        }

        iterator begin() {
            return m_table.begin();
        }

        const_iterator begin() const {
            return m_table.begin();
        }

        iterator end() {
            return m_table.end();
        }

        const_iterator end() const {
            return m_table.end();
        }

        void clear() noexcept {
            m_table.clear();
        }

        template<typename K, typename V>
        pair<iterator, bool> insert(K &&key, V &&val) {
            return m_table.insert_unique(make_tuple(forward<K>(key), forward<V>(val)));
        };

        template<typename K, typename V>
        pair<iterator, bool> insert_or_assign(K &&key, V &&val) {
            pair<iterator, bool> result = m_table.insert_unique(make_tuple(forward<K>(key), forward<V>(val)));
            if (!result.m_second) {
                *result.m_first = forward<V>(val);
            }
            return result;
        };

        iterator erase(const iterator &pos) {
            iterator tmp = pos;
            ++tmp;
            m_table.erase(pos);
            return tmp;
        }

        bool erase(const key_type &key) {
            return m_table.erase(key) > 0;
        }

        val_type &at(const key_type &key) {
            return *m_table.find(key);
        }

        const val_type &at(const key_type &key) const {
            return *m_table.find(key);
        }

        bool contains(const key_type &key) const {
            return m_table.find(key) != m_table.end();
        }

        iterator find(const key_type &key) {
            return m_table.find(key);
        }

        const_iterator find(const key_type &key) const {
            return m_table.find(key);
        }

        template<typename K>
        val_type &operator[](K &&key) {
            pair<iterator, bool> result = m_table.insert_unique(make_tuple(forward<K>(key), val_type()));
            return *result.m_first;
        }

        map_type &operator=(const map_type &) = delete;

        map_type &operator=(map_type &&map) {
            m_table = move(map.m_table);
            return *this;
        }
    };

}

#endif //CORE_STL_MAP_H
