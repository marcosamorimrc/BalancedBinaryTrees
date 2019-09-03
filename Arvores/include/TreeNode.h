/*
   TreeNode.h

   A template for tree nodes.

  Program:   weiss
  Module:    $RCSfile: TreeNode.h,v $
  Language:  C++
  Date:      $Date: 2005/10/09 22:40:44 $
  Version:   $Revision: 1.2 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 15 November 2000.
*/
#ifndef __TNODE
#define __TNODE

#include <iostream>

/** A silly class for testing the tree system.
 *  It should be redefined for your application.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *  @version $Revision: 1.2 $ $Date: 2005/10/09 22:40:44 $
 *
 */	
class TreeNode {
 public: 

  /** @name constructors  */ 
 //@{
 /// empty constructor. 
 TreeNode ( int data = 0 ) { this->data = data; }

 /// copy constructor.
 TreeNode ( const TreeNode& node ) { *this = node; }

 /// destructor.
 ~TreeNode ( ) { }
 //@}

 /// returns data.
 int getData () const { return this->data; }

 /// sets data.
 const TreeNode& setData ( int data ) { this->data = data; return *this; }

 /// a very simple hash function.
 unsigned int hash ( int size ) const { return (13 * this->data) % size; }

 /// operator <.  
 int operator < ( const TreeNode& n ) const {
    return this->data < n.data;
 }

 /// operator ==.  
 int operator == ( const TreeNode& n ) const {
    return this->data == n.data;
 }

 /// operator !=.  
 int operator != ( const TreeNode& n ) const {
    return !(*this == n);
 }

 /// operator >.  
 int operator > ( const TreeNode& n ) const {
    return this->data > n.data;
 }

 /// output operator.
 friend std::ostream& operator<<(std::ostream& os, const TreeNode& nod) {
    os << nod.getData();
    return os;
 }

 private:
   /// the simplest data: just an integer.
   int data;
};

#endif
