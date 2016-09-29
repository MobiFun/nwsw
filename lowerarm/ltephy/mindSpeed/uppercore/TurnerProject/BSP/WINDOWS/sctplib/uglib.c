/** @file uglib.c
*/

#include <stdlib.h>
#include <string.h>
#include <uglib.h>

#define MIN_ARRAY_SIZE  16

typedef struct _GRealArray  GRealArray;

struct _GRealArray
{
  guint8 *data;
  guint   len;
  guint   alloc;
  guint   elt_size;
  guint   zero_terminated : 1;
  guint   clear : 1;
};

void g_list_push_allocator (gpointer dummy) { /* present for binary compat only */ }
void g_list_pop_allocator  (void)           { /* present for binary compat only */ }

#define _g_list_alloc()         g_slice_new (GList)
#define _g_list_alloc0()        g_slice_new0 (GList)
#define _g_list_free1(list)     g_slice_free (GList, list)

gboolean g_mem_gc_friendly = 0;

GList*
g_list_alloc (void)
{
  return _g_list_alloc0 ();
}

/**
 * g_list_free: 
 * @list: a #GList
 *
 * Frees all of the memory used by a #GList.
 * The freed elements are returned to the slice allocator.
 *
 * <note><para>
 * If list elements contain dynamically-allocated memory, 
 * they should be freed first.
 * </para></note>
 */
void
g_list_free (GList *list)
{
  g_slice_free_chain (GList, list, next);
}

/**
 * g_list_free_1:
 * @list: a #GList element
 *
 * Frees one #GList element.
 * It is usually used after g_list_remove_link().
 */
void
g_list_free_1 (GList *list)
{
  _g_list_free1 (list);
}

/**
 * g_list_append:
 * @list: a pointer to a #GList
 * @data: the data for the new element
 *
 * Adds a new element on to the end of the list.
 *
 * <note><para>
 * The return value is the new start of the list, which 
 * may have changed, so make sure you store the new value.
 * </para></note>
 *
 * <note><para>
 * Note that g_list_append() has to traverse the entire list 
 * to find the end, which is inefficient when adding multiple 
 * elements. A common idiom to avoid the inefficiency is to prepend 
 * the elements and reverse the list when all elements have been added.
 * </para></note>
 *
 * |[
 * /&ast; Notice that these are initialized to the empty list. &ast;/
 * GList *list = NULL, *number_list = NULL;
 *
 * /&ast; This is a list of strings. &ast;/
 * list = g_list_append (list, "first");
 * list = g_list_append (list, "second");
 * 
 * /&ast; This is a list of integers. &ast;/
 * number_list = g_list_append (number_list, GINT_TO_POINTER (27));
 * number_list = g_list_append (number_list, GINT_TO_POINTER (14));
 * ]|
 *
 * Returns: the new start of the #GList
 */
GList*
g_list_append (GList	*list,
	       gpointer	 data)
{
  GList *new_list;
  GList *last;
  
  new_list = _g_list_alloc ();
  new_list->data = data;
  new_list->next = NULL;
  
  if (list)
    {
      last = g_list_last (list);
      /* g_assert (last != NULL); */
      last->next = new_list;
      new_list->prev = last;

      return list;
    }
  else
    {
      new_list->prev = NULL;
      return new_list;
    }
}

/**
 * g_list_prepend:
 * @list: a pointer to a #GList
 * @data: the data for the new element
 *
 * Adds a new element on to the start of the list.
 *
 * <note><para>
 * The return value is the new start of the list, which 
 * may have changed, so make sure you store the new value.
 * </para></note>
 *
 * |[ 
 * /&ast; Notice that it is initialized to the empty list. &ast;/
 * GList *list = NULL;
 * list = g_list_prepend (list, "last");
 * list = g_list_prepend (list, "first");
 * ]|
 *
 * Returns: the new start of the #GList
 */
GList*
g_list_prepend (GList	 *list,
		gpointer  data)
{
  GList *new_list;
  
  new_list = _g_list_alloc ();
  new_list->data = data;
  new_list->next = list;
  
  if (list)
    {
      new_list->prev = list->prev;
      if (list->prev)
	list->prev->next = new_list;
      list->prev = new_list;
    }
  else
    new_list->prev = NULL;
  
  return new_list;
}

/**
 * g_list_insert:
 * @list: a pointer to a #GList
 * @data: the data for the new element
 * @position: the position to insert the element. If this is 
 *     negative, or is larger than the number of elements in the 
 *     list, the new element is added on to the end of the list.
 * 
 * Inserts a new element into the list at the given position.
 *
 * Returns: the new start of the #GList
 */
GList*
g_list_insert (GList	*list,
	       gpointer	 data,
	       gint	 position)
{
  GList *new_list;
  GList *tmp_list;
  
  if (position < 0)
    return g_list_append (list, data);
  else if (position == 0)
    return g_list_prepend (list, data);
  
  tmp_list = g_list_nth (list, position);
  if (!tmp_list)
    return g_list_append (list, data);
  
  new_list = _g_list_alloc ();
  new_list->data = data;
  new_list->prev = tmp_list->prev;
  if (tmp_list->prev)
    tmp_list->prev->next = new_list;
  new_list->next = tmp_list;
  tmp_list->prev = new_list;
  
  if (tmp_list == list)
    return new_list;
  else
    return list;
}

/**
 * g_list_insert_before:
 * @list: a pointer to a #GList
 * @sibling: the list element before which the new element 
 *     is inserted or %NULL to insert at the end of the list
 * @data: the data for the new element
 *
 * Inserts a new element into the list before the given position.
 *
 * Returns: the new start of the #GList
 */
GList*
g_list_insert_before (GList   *list,
		      GList   *sibling,
		      gpointer data)
{
  if (!list)
    {
      list = g_list_alloc ();
      list->data = data;
      g_return_val_if_fail (sibling == NULL, list);
      return list;
    }
  else if (sibling)
    {
      GList *node;

      node = _g_list_alloc ();
      node->data = data;
      node->prev = sibling->prev;
      node->next = sibling;
      sibling->prev = node;
      if (node->prev)
	{
	  node->prev->next = node;
	  return list;
	}
      else
	{
	  g_return_val_if_fail (sibling == list, node);
	  return node;
	}
    }
  else
    {
      GList *last;

      last = list;
      while (last->next)
	last = last->next;

      last->next = _g_list_alloc ();
      last->next->data = data;
      last->next->prev = last;
      last->next->next = NULL;

      return list;
    }
}

/**
 * g_list_concat:
 * @list1: a #GList
 * @list2: the #GList to add to the end of the first #GList
 *
 * Adds the second #GList onto the end of the first #GList.
 * Note that the elements of the second #GList are not copied.
 * They are used directly.
 *
 * Returns: the start of the new #GList
 */
GList *
g_list_concat (GList *list1, GList *list2)
{
  GList *tmp_list;
  
  if (list2)
    {
      tmp_list = g_list_last (list1);
      if (tmp_list)
	tmp_list->next = list2;
      else
	list1 = list2;
      list2->prev = tmp_list;
    }
  
  return list1;
}

/**
 * g_list_remove:
 * @list: a #GList
 * @data: the data of the element to remove
 *
 * Removes an element from a #GList.
 * If two elements contain the same data, only the first is removed.
 * If none of the elements contain the data, the #GList is unchanged.
 *
 * Returns: the new start of the #GList
 */
GList*
g_list_remove (GList	     *list,
	       gconstpointer  data)
{
  GList *tmp;
  
  tmp = list;
  while (tmp)
    {
      if (tmp->data != data)
	tmp = tmp->next;
      else
	{
	  if (tmp->prev)
	    tmp->prev->next = tmp->next;
	  if (tmp->next)
	    tmp->next->prev = tmp->prev;
	  
	  if (list == tmp)
	    list = list->next;
	  
	  _g_list_free1 (tmp);
	  
	  break;
	}
    }
  return list;
}

/**
 * g_list_remove_all:
 * @list: a #GList
 * @data: data to remove
 *
 * Removes all list nodes with data equal to @data. 
 * Returns the new head of the list. Contrast with 
 * g_list_remove() which removes only the first node 
 * matching the given data.
 *
 * Returns: new head of @list
 */
GList*
g_list_remove_all (GList	*list,
		   gconstpointer data)
{
  GList *tmp = list;

  while (tmp)
    {
      if (tmp->data != data)
	tmp = tmp->next;
      else
	{
	  GList *next = tmp->next;

	  if (tmp->prev)
	    tmp->prev->next = next;
	  else
	    list = next;
	  if (next)
	    next->prev = tmp->prev;

	  _g_list_free1 (tmp);
	  tmp = next;
	}
    }
  return list;
}

#ifdef _MSC_VER
static __inline
#else
static inline
#endif
GList*
_g_list_remove_link (GList *list,
		     GList *link)
{
  if (link)
    {
      if (link->prev)
	link->prev->next = link->next;
      if (link->next)
	link->next->prev = link->prev;
      
      if (link == list)
	list = list->next;
      
      link->next = NULL;
      link->prev = NULL;
    }
  
  return list;
}

/**
 * g_list_remove_link:
 * @list: a #GList
 * @llink: an element in the #GList
 *
 * Removes an element from a #GList, without freeing the element.
 * The removed element's prev and next links are set to %NULL, so 
 * that it becomes a self-contained list with one element.
 *
 * Returns: the new start of the #GList, without the element
 */
GList*
g_list_remove_link (GList *list,
		    GList *llink)
{
  return _g_list_remove_link (list, llink);
}

/**
 * g_list_delete_link:
 * @list: a #GList
 * @link_: node to delete from @list
 *
 * Removes the node link_ from the list and frees it. 
 * Compare this to g_list_remove_link() which removes the node 
 * without freeing it.
 *
 * Returns: the new head of @list
 */
GList*
g_list_delete_link (GList *list,
		    GList *link_)
{
  list = _g_list_remove_link (list, link_);
  _g_list_free1 (link_);

  return list;
}

/**
 * g_list_copy:
 * @list: a #GList
 *
 * Copies a #GList.
 *
 * <note><para>
 * Note that this is a "shallow" copy. If the list elements 
 * consist of pointers to data, the pointers are copied but 
 * the actual data is not.
 * </para></note>
 *
 * Returns: a copy of @list
 */
GList*
g_list_copy (GList *list)
{
  GList *new_list = NULL;

  if (list)
    {
      GList *last;

      new_list = _g_list_alloc ();
      new_list->data = list->data;
      new_list->prev = NULL;
      last = new_list;
      list = list->next;
      while (list)
	{
	  last->next = _g_list_alloc ();
	  last->next->prev = last;
	  last = last->next;
	  last->data = list->data;
	  list = list->next;
	}
      last->next = NULL;
    }

  return new_list;
}

/**
 * g_list_reverse:
 * @list: a #GList
 *
 * Reverses a #GList.
 * It simply switches the next and prev pointers of each element.
 *
 * Returns: the start of the reversed #GList
 */
GList*
g_list_reverse (GList *list)
{
  GList *last;
  
  last = NULL;
  while (list)
    {
      last = list;
      list = last->next;
      last->next = last->prev;
      last->prev = list;
    }
  
  return last;
}

/**
 * g_list_nth:
 * @list: a #GList
 * @n: the position of the element, counting from 0
 *
 * Gets the element at the given position in a #GList.
 *
 * Returns: the element, or %NULL if the position is off 
 *     the end of the #GList
 */
GList*
g_list_nth (GList *list,
	    guint  n)
{
  while ((n-- > 0) && list)
    list = list->next;
  
  return list;
}

/**
 * g_list_nth_prev:
 * @list: a #GList
 * @n: the position of the element, counting from 0
 *
 * Gets the element @n places before @list.
 *
 * Returns: the element, or %NULL if the position is 
 *     off the end of the #GList
 */
GList*
g_list_nth_prev (GList *list,
		 guint  n)
{
  while ((n-- > 0) && list)
    list = list->prev;
  
  return list;
}

/**
 * g_list_nth_data:
 * @list: a #GList
 * @n: the position of the element
 *
 * Gets the data of the element at the given position.
 *
 * Returns: the element's data, or %NULL if the position 
 *     is off the end of the #GList
 */
gpointer
g_list_nth_data (GList     *list,
		 guint      n)
{
  while ((n-- > 0) && list)
    list = list->next;
  
  return list ? list->data : NULL;
}

/**
 * g_list_find:
 * @list: a #GList
 * @data: the element data to find
 *
 * Finds the element in a #GList which 
 * contains the given data.
 *
 * Returns: the found #GList element, 
 *     or %NULL if it is not found
 */
GList*
g_list_find (GList         *list,
	     gconstpointer  data)
{
  while (list)
    {
      if (list->data == data)
	break;
      list = list->next;
    }
  
  return list;
}

/**
 * g_list_find_custom:
 * @list: a #GList
 * @data: user data passed to the function
 * @func: the function to call for each element. 
 *     It should return 0 when the desired element is found
 *
 * Finds an element in a #GList, using a supplied function to 
 * find the desired element. It iterates over the list, calling 
 * the given function which should return 0 when the desired 
 * element is found. The function takes two #gconstpointer arguments, 
 * the #GList element's data as the first argument and the 
 * given user data.
 *
 * Returns: the found #GList element, or %NULL if it is not found
 */
GList*
g_list_find_custom (GList         *list,
		    gconstpointer  data,
		    GCompareFunc   func)
{
  g_return_val_if_fail (func != NULL, list);

  while (list)
    {
      if (! func (list->data, data))
	return list;
      list = list->next;
    }

  return NULL;
}


/**
 * g_list_position:
 * @list: a #GList
 * @llink: an element in the #GList
 *
 * Gets the position of the given element 
 * in the #GList (starting from 0).
 *
 * Returns: the position of the element in the #GList, 
 *     or -1 if the element is not found
 */
gint
g_list_position (GList *list,
		 GList *llink)
{
  gint i;

  i = 0;
  while (list)
    {
      if (list == llink)
	return i;
      i++;
      list = list->next;
    }

  return -1;
}

/**
 * g_list_index:
 * @list: a #GList
 * @data: the data to find
 *
 * Gets the position of the element containing 
 * the given data (starting from 0).
 *
 * Returns: the index of the element containing the data, 
 *     or -1 if the data is not found
 */
gint
g_list_index (GList         *list,
	      gconstpointer  data)
{
  gint i;

  i = 0;
  while (list)
    {
      if (list->data == data)
	return i;
      i++;
      list = list->next;
    }

  return -1;
}

/**
 * g_list_last:
 * @list: a #GList
 *
 * Gets the last element in a #GList.
 *
 * Returns: the last element in the #GList, 
 *     or %NULL if the #GList has no elements
 */
GList*
g_list_last (GList *list)
{
  if (list)
    {
      while (list->next)
	list = list->next;
    }
  
  return list;
}

/**
 * g_list_first:
 * @list: a #GList
 *
 * Gets the first element in a #GList.
 *
 * Returns: the first element in the #GList, 
 *     or %NULL if the #GList has no elements
 */
GList*
g_list_first (GList *list)
{
  if (list)
    {
      while (list->prev)
	list = list->prev;
    }
  
  return list;
}

/**
 * g_list_length:
 * @list: a #GList
 *
 * Gets the number of elements in a #GList.
 *
 * <note><para>
 * This function iterates over the whole list to 
 * count its elements.
 * </para></note>
 *
 * Returns: the number of elements in the #GList
 */
guint
g_list_length (GList *list)
{
  guint length;
  
  length = 0;
  while (list)
    {
      length++;
      list = list->next;
    }
  
  return length;
}

/**
 * g_list_foreach:
 * @list: a #GList
 * @func: the function to call with each element's data
 * @user_data: user data to pass to the function
 *
 * Calls a function for each element of a #GList.
 */
void
g_list_foreach (GList	 *list,
		GFunc	  func,
		gpointer  user_data)
{
  while (list)
    {
      GList *next = list->next;
      (*func) (list->data, user_data);
      list = next;
    }
}

static GList*
g_list_insert_sorted_real (GList    *list,
			   gpointer  data,
			   GFunc     func,
			   gpointer  user_data)
{
  GList *tmp_list = list;
  GList *new_list;
  gint cmp;

  g_return_val_if_fail (func != NULL, list);
  
  if (!list) 
    {
      new_list = _g_list_alloc0 ();
      new_list->data = data;
      return new_list;
    }
  
  cmp = ((GCompareDataFunc) func) (data, tmp_list->data, user_data);

  while ((tmp_list->next) && (cmp > 0))
    {
      tmp_list = tmp_list->next;

      cmp = ((GCompareDataFunc) func) (data, tmp_list->data, user_data);
    }

  new_list = _g_list_alloc0 ();
  new_list->data = data;

  if ((!tmp_list->next) && (cmp > 0))
    {
      tmp_list->next = new_list;
      new_list->prev = tmp_list;
      return list;
    }
   
  if (tmp_list->prev)
    {
      tmp_list->prev->next = new_list;
      new_list->prev = tmp_list->prev;
    }
  new_list->next = tmp_list;
  tmp_list->prev = new_list;
 
  if (tmp_list == list)
    return new_list;
  else
    return list;
}

/**
 * g_list_insert_sorted:
 * @list: a pointer to a #GList
 * @data: the data for the new element
 * @func: the function to compare elements in the list. It should 
 *     return a number > 0 if the first parameter comes after the 
 *     second parameter in the sort order.
 *
 * Inserts a new element into the list, using the given comparison 
 * function to determine its position.
 *
 * Returns: the new start of the #GList
 */
GList*
g_list_insert_sorted (GList        *list,
		      gpointer      data,
		      GCompareFunc  func)
{
  return g_list_insert_sorted_real (list, data, (GFunc) func, NULL);
}

/**
 * g_list_insert_sorted_with_data:
 * @list: a pointer to a #GList
 * @data: the data for the new element
 * @func: the function to compare elements in the list. 
 *     It should return a number > 0 if the first parameter 
 *     comes after the second parameter in the sort order.
 * @user_data: user data to pass to comparison function.
 *
 * Inserts a new element into the list, using the given comparison 
 * function to determine its position.
 *
 * Returns: the new start of the #GList
 *
 * Since: 2.10
 */
GList*
g_list_insert_sorted_with_data (GList            *list,
				gpointer          data,
				GCompareDataFunc  func,
				gpointer          user_data)
{
  return g_list_insert_sorted_real (list, data, (GFunc) func, user_data);
}

static GList *
g_list_sort_merge (GList     *l1, 
		   GList     *l2,
		   GFunc     compare_func,
		   gpointer  user_data)
{
  GList list, *l, *lprev;
  gint cmp;

  l = &list; 
  lprev = NULL;

  while (l1 && l2)
    {
      cmp = ((GCompareDataFunc) compare_func) (l1->data, l2->data, user_data);

      if (cmp <= 0)
        {
	  l->next = l1;
	  l1 = l1->next;
        } 
      else 
	{
	  l->next = l2;
	  l2 = l2->next;
        }
      l = l->next;
      l->prev = lprev; 
      lprev = l;
    }
  l->next = l1 ? l1 : l2;
  l->next->prev = l;

  return list.next;
}

static GList* 
g_list_sort_real (GList    *list,
		  GFunc     compare_func,
		  gpointer  user_data)
{
  GList *l1, *l2;
  
  if (!list) 
    return NULL;
  if (!list->next) 
    return list;
  
  l1 = list; 
  l2 = list->next;

  while ((l2 = l2->next) != NULL)
    {
      if ((l2 = l2->next) == NULL) 
	break;
      l1 = l1->next;
    }
  l2 = l1->next; 
  l1->next = NULL; 

  return g_list_sort_merge (g_list_sort_real (list, compare_func, user_data),
			    g_list_sort_real (l2, compare_func, user_data),
			    compare_func,
			    user_data);
}

/**
 * g_list_sort:
 * @list: a #GList
 * @compare_func: the comparison function used to sort the #GList.
 *     This function is passed the data from 2 elements of the #GList 
 *     and should return 0 if they are equal, a negative value if the 
 *     first element comes before the second, or a positive value if 
 *     the first element comes after the second.
 *
 * Sorts a #GList using the given comparison function.
 *
 * Returns: the start of the sorted #GList
 */
GList *
g_list_sort (GList        *list,
	     GCompareFunc  compare_func)
{
  return g_list_sort_real (list, (GFunc) compare_func, NULL);
			    
}

/**
 * g_list_sort_with_data:
 * @list: a #GList
 * @compare_func: comparison function
 * @user_data: user data to pass to comparison function
 *
 * Like g_list_sort(), but the comparison function accepts 
 * a user data argument.
 *
 * Returns: the new head of @list
 */
GList *
g_list_sort_with_data (GList            *list,
		       GCompareDataFunc  compare_func,
		       gpointer          user_data)
{
  return g_list_sort_real (list, (GFunc) compare_func, user_data);
}

#define g_array_elt_len(array,i) ((array)->elt_size * (i))

#define g_array_elt_pos(array,i) ((array)->data + g_array_elt_len((array),(i)))

#define g_array_elt_zero(array, pos, len) 				\
  (memset (g_array_elt_pos ((array), pos), 0,  g_array_elt_len ((array), len)))

#define g_array_zero_terminate(array) G_STMT_START{			\
  if ((array)->zero_terminated)						\
    g_array_elt_zero ((array), (array)->len, 1);			\
}G_STMT_END

static void g_array_maybe_expand (GRealArray *array,
				  gint        len);
static gint g_nearest_pow        (gint        num);

GArray*
g_array_new (gboolean zero_terminated,
	     gboolean clear,
	     guint    elt_size)
{
  return (GArray*) g_array_sized_new (zero_terminated, clear, elt_size, 0);
}

GArray* g_array_sized_new (gboolean zero_terminated,
			   gboolean clear,
			   guint    elt_size,
			   guint    reserved_size)
{
  GRealArray *array = g_slice_new (GRealArray);

  array->data            = NULL;
  array->len             = 0;
  array->alloc           = 0;
  array->zero_terminated = (zero_terminated ? 1 : 0);
  array->clear           = (clear ? 1 : 0);
  array->elt_size        = elt_size;

  if (array->zero_terminated || reserved_size != 0)
    {
      g_array_maybe_expand (array, reserved_size);
      g_array_zero_terminate(array);
    }

  return (GArray*) array;
}

gchar*
g_array_free (GArray  *array,
	      gboolean free_segment)
{
  gchar* segment;

  g_return_val_if_fail (array, NULL);

  if (free_segment)
    {
      g_free (array->data);
      segment = NULL;
    }
  else
    segment = array->data;

  g_slice_free1 (sizeof (GRealArray), array);

  return segment;
}

static void
g_array_maybe_expand (GRealArray *array,
		      gint        len)
{
  guint want_alloc = g_array_elt_len (array, array->len + len + 
				      array->zero_terminated);

  if (want_alloc > array->alloc)
    {
      want_alloc = g_nearest_pow (want_alloc);
      want_alloc = MAX (want_alloc, MIN_ARRAY_SIZE);

      array->data = g_realloc (array->data, want_alloc);

      if (G_UNLIKELY (g_mem_gc_friendly))
        memset (array->data + array->alloc, 0, want_alloc - array->alloc);

      array->alloc = want_alloc;
    }
}

static gint
g_nearest_pow (gint num)
{
  gint n = 1;

  while (n < num)
    n <<= 1;

  return n;
}

GArray*
g_array_insert_vals (GArray        *farray,
		     guint          index,
		     gconstpointer  data,
		     guint          len)
{
  GRealArray *array = (GRealArray*) farray;

  g_array_maybe_expand (array, len);

  g_memmove (g_array_elt_pos (array, len + index), 
	     g_array_elt_pos (array, index), 
	     g_array_elt_len (array, array->len - index));

  memcpy (g_array_elt_pos (array, index), data, g_array_elt_len (array, len));

  array->len += len;

  g_array_zero_terminate (array);

  return farray;
}

GArray*
g_array_append_vals (GArray       *farray,
		     gconstpointer data,
		     guint         len)
{
  GRealArray *array = (GRealArray*) farray;

  g_array_maybe_expand (array, len);

  memcpy (g_array_elt_pos (array, array->len), data, 
	  g_array_elt_len (array, len));

  array->len += len;

  g_array_zero_terminate (array);

  return farray;
}

gpointer
g_slice_alloc0 (gsize mem_size)
{
  gpointer mem = g_slice_alloc (mem_size);
  if (mem)
    memset (mem, 0, mem_size);
  return mem;
}

void
g_slice_free_chain_with_offset (gsize    mem_size,
                                gpointer mem_chain,
                                gsize    next_offset)
{
  gpointer slice = mem_chain;

  while (slice)
      {
        guint8 *current = slice;
        slice = *(gpointer*) (current + next_offset);
        if (G_UNLIKELY (g_mem_gc_friendly))
          memset (current, 0, mem_size);
        g_free (current);
      }
}

void
g_slice_free1 (gsize    mem_size,
               gpointer mem_block)
{
      if (G_UNLIKELY (g_mem_gc_friendly))
        memset (mem_block, 0, mem_size);
      g_free (mem_block);
}

gpointer
g_malloc (gsize n_bytes)
{
      gpointer mem;

      mem = malloc (n_bytes);
      if (mem)
	    return mem;

  return NULL;
}

gpointer
g_slice_alloc (gsize mem_size)
{
  gpointer mem;

  mem = g_malloc (mem_size);

  return mem;
}

gpointer
g_realloc (gpointer mem,
	   gsize    n_bytes)
{
    return realloc(mem, n_bytes);
}

void
g_free (gpointer mem)
{
    free(mem);
}
