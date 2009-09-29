/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/* 
 * File:   list.h
 * Author: Ali Mosavian
 *
 * Created on July 22, 2009
 */

#ifndef __MOSTL_LIST_H__
#define	__MOSTL_LIST_H__

#include "common.h"
#include "algorithm.h"

NAMESPACE_BEGIN ( mostd )

template <typename _Tp>
class list
{
private:
	struct _List_Node_;
    typedef _List_Node_ _node;

    _node * m_head;
    _node * m_tail;


    struct _List_Node_
    {
        _node* m_next;
        _node* m_prev;
        _Tp m_data;

        /**
         * Constructor
         *
         * Inserts itself inbetween p and n and updates the list
         * head and tail if required.
         *
         * @param d     The actual node data
         * @param p     Pointer to the next node
         * @param n     Pointer to the previous node
         */
         _List_Node_ ( _Tp d, _node *p, _node *n );

        /**
         * Destructor
         *
         * Will link together the front and back links and update
         * head and tail if required.
         */
        ~_List_Node_ ( void );
    };

public:
	struct _List_Iterator_;
    typedef _List_Iterator_  iterator;

    struct _List_Iterator_
    {
        typedef _List_Iterator_ _self;
        typedef _Tp & _refrence;
        typedef _Tp & _pointer;

        _node*  m_node;

        /**
         * Constructor
         *
         */
        explicit _List_Iterator_ ( _node *n )
        : m_node( n )
        { }

        /**
         * Copy constructor
         *
         */
         _List_Iterator_ ( _self &it )
        : m_node( it.m_node )
        { }

        /**
         * De-refrence operator
         *
         * @return Refrence to type _Tp
         */
        _refrence operator* ( void ) const
        {
            return m_node->m_data;
        }

        /**
         * De-refrence operator
         *
         * @return Pointer to type _Tp
         */
        _pointer operator-> ( void ) const
        {
            return &(m_node->m_data);
        }

        /**
         * Prefix increments the iterator
         *
         * @return Itself
         */
        _self & operator++ ( void )
        {
            m_node = m_node->m_next;
            return *this;
        }
        
        /**
         * Postfix increments the iterator
         *
         * @return Copy of the iterator before it was advanced
         */
        _self operator++ ( int )
        {
            _self tmp( *this );
            m_node = m_node->m_next;
            return tmp;
        }

        /**
         * Prefix decrements the iterator
         *
         * @return Itself
         */
        _self & operator-- ( void )
        {
            m_node = m_node->m_prev;
            return *this;
        }

        /**
         * Postfix decrements the iterator
         *
         * @return Copy of the iterator before it was advanced
         */
        _self operator-- ( int )
        {
            _self tmp( *this );
            m_node = m_node->m_prev;
            return tmp;
        }

        /**
         * Equal operator
         *
         */
        bool operator== ( const _self & x ) const
        {
            return m_node->m_data == x.m_node->m_data;
        }

        /**
         * Not Equal operator
         *
         */
        bool operator!= ( const _self & x ) const
        {
            return m_node->m_data != x.m_node->m_data;
        }
    };

    /**
     * Constructor
     *
     */
    list ( void )
    : m_head( NULL ), m_tail( NULL )
    { }

    /**
     *  Returns a read/write iterator that points to the first element in the
     *  %list.  Iteration is done in ordinary element order.
     */
    iterator begin ( void )
    {
        return iterator( m_head );
    }

    /**
     *  Returns a read/write iterator that points one past the last
     *  element in the %list.  Iteration is done in ordinary element
     *  order.
     */
    iterator end ( void )
    {
        return iterator( m_tail );
    }

    /**
     *  Returns true if the %list is empty.  (Thus begin() would equal
     *  end().)
     */
    bool empty ( void ) const
    {
        return m_head == NULL;
    }

    /**
     * Returns the number of elements in the %list.
     *
     */
    unsigned long size ( void ) const;

    /**
     *  Returns a read/write reference to the data at the first
     *  element of the %list.
     */
    _Tp& front ( void )
    {
        return m_head->m_data;
    }

    /**
     *  Returns a read/write reference to the data at the last element
     *  of the %list.
     */
    _Tp& back ( void )
    {
        return m_tail->m_data;
    }

    /**
     *  @brief  Add data to the front of the %list.
     *  @param  x  Data to be added.
     *
     *  This is a typical stack operation.  The function creates an
     *  element at the front of the %list and assigns the given data
     *  to it.  Due to the nature of a %list this operation can be
     *  done in constant time, and does not invalidate iterators and
     *  references.
     */
    void push_front ( const _Tp& x )
    {
        // Constructor will handle insertion
         _node *n = new _node( x, NULL, m_head );

        // FIXME: Check if allocation failed
    }


    /**
     *  @brief  Removes first element.
     *
     *  This is a typical stack operation.  It shrinks the %list by
     *  one.  Due to the nature of a %list this operation can be done
     *  in constant time, and only invalidates iterators/references to
     *  the element being removed.
     *
     *  Note that no data is returned, and if the first element's data
     *  is needed, it should be retrieved before pop_front() is
     *  called.
     */
    void pop_front ( void )
    {
      // Destructor will handle removal
      delete m_head;
    }

    /**
     *  @brief  Add data to the end of the %list.
     *  @param  x  Data to be added.
     *
     *  This is a typical stack operation.  The function creates an
     *  element at the end of the %list and assigns the given data to
     *  it.  Due to the nature of a %list this operation can be done
     *  in constant time, and does not invalidate iterators and
     *  references.
     */
    void push_back ( _Tp& x )
    {
        // Constructor will handle insertion
         _node *n = new _node( x, m_tail, NULL );

		// FIXME: Check if allocation failed
    }

    /**
     *  @brief  Removes last element.
     *
     *  This is a typical stack operation.  It shrinks the %list by
     *  one.  Due to the nature of a %list this operation can be done
     *  in constant time, and only invalidates iterators/references to
     *  the element being removed.
     *
     *  Note that no data is returned, and if the last element's data
     *  is needed, it should be retrieved before pop_back() is called.
     */
    void pop_back ( void )
    {
      // Destructor will handle removal
      delete m_tail;
    }

    /**
     *  @brief  Inserts given value into %list before specified iterator.
     *  @param  p  An iterator into the %list.
     *  @param  x  Data to be inserted.
     *  @return  An iterator that points to the inserted data.
     *
     *  This function will insert a copy of the given value before
     *  the specified location.  Due to the nature of a %list this
     *  operation can be done in constant time, and does not
     *  invalidate iterators and references.
     */
    iterator insert ( iterator &p, const _Tp& x )
    {
        // Constructor will handle the insertion
        _node *n = new _node( x, p.m_node.m_prev, p.m_node.m_next );

        // FIXME: Check if allocation failed here
        
        return iterator( n );
    }

    /**
     *  @brief  Remove element at given position.
     *  @param  p  Iterator pointing to element to be erased.
     *  @return  An iterator pointing to the next element (or end()).
     *
     *  This function will erase the element at the given position and thus
     *  shorten the %list by one.
     *
     *  Due to the nature of a %list this operation can be done in
     *  constant time, and only invalidates iterators/references to
     *  the element being removed.  The user is also cautioned that
     *  this function only erases the element, and that if the element
     *  is itself a pointer, the pointed-to memory is not touched in
     *  any way.  Managing the pointer is the user's responsibility.
     */
    iterator erase ( iterator &p )
    {
        _node *n = p.m_node->m_next;

        // The destructor will handle the removal
        delete p.m_node;

        return iterator( n );
    }

    /**
     *  @brief  Remove a range of elements.
     *  @param  first  Iterator pointing to the first element to be erased.
     *  @param  last  Iterator pointing to one past the last element to be
     *                erased.
     *  @return  An iterator pointing to the element pointed to by @a last
     *           prior to erasing (or end()).
     *
     *  This function will erase the elements in the range @a
     *  [first,last) and shorten the %list accordingly.
     *
     *  This operation is linear time in the size of the range and only
     *  invalidates iterators/references to the element being removed.
     *  The user is also cautioned that this function only erases the
     *  elements, and that if the elements themselves are pointers, the
     *  pointed-to memory is not touched in any way.  Managing the pointer
     *  is the user's responsibility.
     */
    iterator erase ( iterator &first, iterator &last )
	{
		while ( first != last )
		  first = erase( first );

		return last;
	}


    /**
     *  @brief  Swaps data with another %list.
     *  @param  x  A %list of the same element and allocator types.
     *
     *  This exchanges the elements between two lists in constant
     *  time.  Note that the global std::swap() function is
     *  specialized such that std::swap(l1,l2) will feed to this
     *  function.
     */
    void swap ( list<_Tp> &x )
    {
        swap<_Tp>( m_head, x.m_head );
        swap<_Tp>( m_tail, x.m_tail );
    }

    /**
     *  Erases all the elements.  Note that this function only erases
     *  the elements, and that if the elements themselves are
     *  pointers, the pointed-to memory is not touched in any way.
     *  Managing the pointer is the user's responsibility.
     */
    void clear ( void );


    /**
     *  @brief  Remove all elements equal to value.
     *  @param  value  The value to remove.
     *
     *  Removes every element in the list equal to @a value.
     *  Remaining elements stay in list order.  Note that this
     *  function only erases the elements, and that if the elements
     *  themselves are pointers, the pointed-to memory is not
     *  touched in any way.  Managing the pointer is the user's
     *  responsibility.
     */
    void remove ( const _Tp& value )
    {
        _node *n = m_head;
        while ( n != NULL )
        {
            if ( n->m_data != value )
                continue;

            // The destructor will handle removal
            delete n;
            break;
        }
    }

};


template <typename _Tp>
list<_Tp>::_List_Node_::_List_Node_ ( const _Tp d, _node *p, _node *n )
{
    m_data = d;

    // Check if head
    m_prev = p;
    if ( p == NULL )
        m_head = this;
    else
        p->m_next = this;

    // Check if tail
    m_next = n;
    if ( n == NULL )
        m_tail = this;
    else
        n->m_prev = this;
}

template <typename _Tp>
list<_Tp>::_List_Node_::~_List_Node_ ( void )
{
    // Check if head
    if ( m_prev == NULL )
        m_head = m_next;
    else
        m_prev->m_next = m_next;

    // Check if tail
    if ( m_next == NULL )
        m_tail = m_prev;
    else
        m_next->m_prev = m_prev;
}

template <typename _Tp>
unsigned long list<_Tp>::size ( void ) const
{
    unsigned int s = 0;
    _node n = m_head;

    while ( n != NULL )
    {
        n = n->m_next;
        s++;
    }

    return s;
}

template <typename _Tp>
void list<_Tp>::clear ( void )
{
    _node *t;
    _node *n = m_head;

    // The node destructor will handle removal
    while ( n != NULL )
    {
        t = n;
        n = n->m_next;
        delete t;
    }
}


NAMESPACE_END

#endif	/* __MOSTL_LIST_H__ */
