#ifndef __AbsBst
#define __AbsBst

/** SearchTree abstract class interface.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 * Etype: must have zero-parameter constructor; <br>
 *     must have operator< and operator==; implementation. <br>
 *     may require operator> and operator!=; <br>
 *     implementation will require either. <br>
 *     operator= or copy constructor, perhaps both. <br>
 * CONSTRUCTION: with (a) no initializer; <br>
 *     copy constructor of SearchTree objects is DISALLOWED. <br>
 *
 * ******************PUBLIC OPERATIONS********************* <br>
 *     All of the following are pure virtual functions <br>
 * int Insert( const Etype& X )  --> Insert X <br>
 * int Remove( const Etype& X )  --> Remove X <br>
 * Etype Find( const Etype& X )  --> Return item that matches X <br>
 * int WasFound( )               --> Return 1 if last Find succeeded <br>
 * int IsFound( const Etype& X ) --> Return 1 if X would be found <br>
 * Etype FindMin( )              --> Return smallest item <br>
 * Etype FindMax( )              --> Return largest item <br>
 * int IsEmpty( )                --> Return 1 if empty; else return 0 <br>
 * int IsFull( )                 --> Retrun 1 if full; else return 0 <br>
 * void MakeEmpty( )             --> Remove all items <br>
 */

template <class Etype, class Cmp>
class AbsBst
{
  public:

  /** @name constructors  */ 
  //@{
    /// empty constructor.
    AbsBst( ) { }
    /// destructor.
    virtual ~AbsBst( ) { }
  //@}

    /// inserts a given element into the tree.
    virtual int Insert( const Etype& X ) = 0;
    /// removes a given element from the tree.
    virtual int Remove( const Etype& X ) = 0;
    /// finds a given element in the tree.
    virtual const Etype& Find ( const Etype& X ) const = 0;
    /** returns whether a given element was found in the tree, 
     *  since the last call to Find().
     */
    virtual int WasFound( ) const = 0;
    /// finds minimum element in tree.
    virtual const Etype& FindMin( ) const = 0;
    /// finds maximum element in tree.
    virtual const Etype& FindMax( ) const = 0;
    /// returns whether the given element is in the tree.
    virtual int IsFound( const Etype& X ) const = 0;
    /// returns whether this tree is empty.
    virtual int IsEmpty( ) const = 0;
    /// returns whether this tree is full.
    virtual int IsFull( ) const = 0; 
    /// makes this tree empty.
    virtual void MakeEmpty( ) = 0;

  private:
    /// Disable copy constructor.
    AbsBst( const AbsBst & ) { }
};
#endif
