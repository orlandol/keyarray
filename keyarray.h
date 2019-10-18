#ifndef KEYARRAY_H
#define KEYARRAY_H

/*
MIT License

Copyright (c) 2019 Orlando Llanes

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdlib.h>
#include <string.h>

/*
 * =================================
 *  String Key Array implementation
 * =================================
 */

  #define DECLARE_STRING_KEYARRAY_TYPES(\
      typeName, dataType )\
  typedef struct typeName##Item {\
    char* key;\
    dataType data;\
  } typeName##Item;\
  \
  typedef struct typeName {\
    size_t reservedCount;\
    size_t itemCount;\
    typeName##Item* item;\
  } typeName;

  #define DECLARE_STRING_KEYARRAY_CREATE( funcName, listType )\
  listType* funcName( size_t reserveCount ) {\
      listType* newKeyArray = NULL;\
    \
    newKeyArray = (listType*)calloc(1, sizeof(listType));\
    if( newKeyArray == NULL ) {\
      goto ReturnError;\
    }\
    \
    if( reserveCount ) {\
      newKeyArray->item =\
        (listType##Item*)calloc(reserveCount, sizeof(listType##Item));\
      if( newKeyArray->item == NULL ) {\
        goto ReturnError;\
      }\
      \
      newKeyArray->reservedCount = reserveCount;\
    }\
    return newKeyArray;\
  \
  ReturnError:\
    if( newKeyArray ) {\
      if( newKeyArray->item ) {\
        free( newKeyArray->item );\
        newKeyArray->item = NULL;\
      }\
      free( newKeyArray );\
      newKeyArray = NULL;\
    }\
    return NULL;\
  }

  #define DECLARE_STRING_KEYARRAY_FREE( funcName, listType, freeDataFunc )\
  void funcName( listType** keyList ) {\
    size_t index;\
    size_t itemCount;\
    \
    if( keyList && (*keyList) ) {\
      itemCount = (*keyList)->itemCount;\
      for( index = 0; index < itemCount; index++ ) {\
        if( (*keyList)->item[index].key ) {\
          free( (*keyList)->item[index].key );\
        }\
        freeDataFunc( &((*keyList)->item[index].data) );\
      }\
      \
      free( (*keyList) );\
      (*keyList) = NULL;\
    }\
  }

  #define DECLARE_STRING_KEYARRAY_INSERT( funcName, listType, dataType )\
  int funcName( listType* keyList, char* key, dataType* data ) {\
    unsigned leftIndex;\
    unsigned insertIndex;\
    unsigned rightIndex;\
    int result;\
    char* newStrKey;\
    size_t keyLen;\
    unsigned prevCount;\
    unsigned reservedCount;\
    unsigned itemCount;\
    listType##Item* item;\
    \
    if( !(keyList && key && data) ) {\
      return 0;\
    }\
    \
    keyLen = strlen(key);\
    if( keyLen == 0 ) {\
      return 0;\
    }\
    \
    /* Grow list, if necessary */\
    reservedCount = keyList->reservedCount;\
    itemCount = keyList->itemCount;\
    item = keyList->item;\
    \
    if( itemCount == reservedCount ) {\
      prevCount = reservedCount;\
      reservedCount += 8;\
      if( prevCount > reservedCount ) {\
        return 0;\
      }\
      \
      item = realloc(item, reservedCount * sizeof(listType##Item));\
      if( item == NULL ) {\
        return 0;\
      }\
      keyList->reservedCount = reservedCount;\
      keyList->item = item;\
    }\
    \
    /* Search for insert position */\
    leftIndex = 0;\
    rightIndex = itemCount;\
    insertIndex = itemCount / 2;\
    \
    while( leftIndex < rightIndex ) {\
      result = strcmp(item[insertIndex].key, key);\
      \
      if( result == 0 ) {\
        return 0;\
      }\
      \
      if( result > 0 ) {\
        rightIndex = insertIndex;\
      } else {\
        leftIndex = insertIndex + 1;\
      }\
      \
      insertIndex = (leftIndex + rightIndex) / 2;\
    }\
    \
    /* Attempt to allocate key string before going further */\
    newStrKey = malloc(keyLen + 1);\
    if( newStrKey == NULL ) {\
      return 0;\
    }\
    strcpy( newStrKey, key );\
    \
    /* Move data past insertion point up, if necessary */\
    memmove( &(item[insertIndex + 1]), &(item[insertIndex]),\
        (itemCount - insertIndex) * sizeof(listType##Item) );\
    \
    /* Insert item */\
    item[insertIndex].key = newStrKey;\
    if( data ) {\
      memcpy( &(item[insertIndex].data), data, sizeof(dataType) );\
    }\
    \
    keyList->itemCount++;\
    \
    return 1;\
  }

  #define DECLARE_STRING_KEYARRAY_REMOVE( funcName, listType, freeDataFunc )\
  void funcName( listType* keyList, char* key ) {\
    unsigned leftIndex;\
    unsigned rightIndex;\
    unsigned removeIndex;\
    int result;\
    unsigned reservedCount;\
    unsigned itemCount;\
    listType##Item* item;\
    \
    if( !(keyList && keyList->item && key && (*key)) ) {\
      return;\
    }\
    \
    reservedCount = keyList->reservedCount;\
    itemCount = keyList->itemCount;\
    item = keyList->item;\
    \
    /* Search for insert position */\
    leftIndex = 0;\
    rightIndex = itemCount;\
    removeIndex = itemCount / 2;\
    \
    while( leftIndex < rightIndex ) {\
      result = strcmp(item[removeIndex].key, key);\
      \
      if( result == 0 ) {\
        freeDataFunc( &(item[removeIndex].data) );\
        if( item[removeIndex].key ) {\
          free( item[removeIndex].key );\
          item[removeIndex].key = NULL;\
        }\
        \
        if( itemCount ) {\
          itemCount--;\
          \
          memcpy( &(item[removeIndex]), &(item[removeIndex + 1]),\
            (itemCount - removeIndex) * sizeof(listType##Item) );\
          \
          keyList->itemCount = itemCount;\
        }\
        \
        memset( &(item[itemCount]), 0, sizeof(listType##Item) );\
        \
        return;\
      }\
      \
      if( result > 0 ) {\
        rightIndex = removeIndex;\
      } else {\
        leftIndex = removeIndex + 1;\
      }\
      \
      removeIndex = (leftIndex + rightIndex) / 2;\
    }\
  }

  #define DECLARE_STRING_KEYARRAY_RETRIEVE( funcName, listType, dataType )\
  int funcName( listType* keyList, char* key, dataType* destData ) {\
    unsigned leftIndex;\
    unsigned rightIndex;\
    unsigned retrieveIndex;\
    int result;\
    unsigned reservedCount;\
    unsigned itemCount;\
    listType##Item* item;\
    \
    if( !(keyList && keyList->item && key && (*key) && destData) ) {\
      return 0;\
    }\
    \
    reservedCount = keyList->reservedCount;\
    itemCount = keyList->itemCount;\
    item = keyList->item;\
    \
    /* Search for item */\
    leftIndex = 0;\
    rightIndex = itemCount;\
    retrieveIndex = itemCount / 2;\
    \
    while( leftIndex < rightIndex ) {\
      result = strcmp(item[retrieveIndex].key, key);\
      \
      if( result == 0 ) {\
        memcpy( destData, &(item[retrieveIndex].data), sizeof(dataType) );\
        return 1;\
      }\
      \
      if( result > 0 ) {\
        rightIndex = retrieveIndex;\
      } else {\
        leftIndex = retrieveIndex + 1;\
      }\
      \
      retrieveIndex = (leftIndex + rightIndex) / 2;\
    }\
    \
    return 0;\
  }

  #define DECLARE_STRING_KEYARRAY_RELEASEUNUSED( funcName, listType )\
  void funcName( listType* keyList ) {\
    listType##Item* item;\
    \
    if( keyList == NULL ) {\
      return;\
    }\
    \
    if( keyList->item && keyList->itemCount ) {\
      /* Resize to remove reserved space*/\
      item = realloc(keyList->item,\
        keyList->itemCount * sizeof(listType##Item));\
      if( item ) {\
        keyList->item = item;\
        keyList->reservedCount = keyList->itemCount;\
      }\
    } else {\
      /* Deallocate */\
      keyList->reservedCount = 0;\
      keyList->itemCount = 0;\
      if( keyList->item ) {\
        free( keyList->item );\
        keyList->item = NULL;\
      }\
    }\
  }

  #define DECLARE_STRING_KEYARRAY_COPY( funcName, listType, dataType,\
      copyDataFunc, freeDataFunc )\
  listType* funcName( listType* sourceList ) {\
    listType* newCopy = NULL;\
    listType##Item* sourceItem = NULL;\
    size_t reservedCount = 0;\
    size_t itemCount = 0;\
    char* keyCopy;\
    size_t keyLen;\
    size_t index;\
    \
    if( sourceList == NULL ) {\
      return NULL;\
    }\
    \
    /* Attempt to allocate list object */\
    newCopy = calloc(1, sizeof(listType));\
    if( newCopy == NULL ) {\
      goto ReturnError;\
    }\
    \
    /* Initialize important variables */\
    reservedCount = sourceList->reservedCount;\
    itemCount = sourceList->itemCount;\
    sourceItem = sourceList->item;\
    \
    /* A list with no items is valid */\
    if( !(reservedCount && itemCount && sourceItem) ) {\
      return newCopy;\
    }\
    \
    /* Copy data, then copy the string keys */\
    newCopy->item = malloc(reservedCount * sizeof(listType##Item));\
    if( newCopy->item == NULL ) {\
      goto ReturnError;\
    }\
    \
    for( index = 0; index < itemCount; index++ ) {\
      copyDataFunc( &(newCopy->item[index].data),\
          &(sourceItem[index].data) );\
      \
      keyLen = strlen(sourceItem[index].key);\
      keyCopy = malloc(keyLen + 1);\
      if( keyCopy == NULL ) {\
        goto ReturnError;\
      }\
      strcpy( keyCopy, sourceItem[index].key );\
      \
      newCopy->item[index].key = keyCopy;\
    }\
    \
    newCopy->reservedCount = reservedCount;\
    newCopy->itemCount = itemCount;\
    \
    return newCopy;\
    \
  ReturnError:\
    if( newCopy == NULL ) {\
      return NULL;\
    }\
    \
    if( newCopy->item ) {\
      for( index = 0; index < itemCount; index++ ) {\
        freeDataFunc( &(newCopy->item[index].data) );\
        \
        keyCopy = newCopy->item[index].key;\
        if( keyCopy ) {\
          free( keyCopy );\
          keyCopy = NULL;\
        }\
      }\
    }\
    \
    free( newCopy );\
    newCopy = NULL;\
    \
    return NULL;\
  }

/*
 * ===================================
 *  Unsigned Key Array implementation
 * ===================================
 */

  #define DECLARE_UINT_KEYARRAY_TYPES(\
      typeName, dataType )\
  typedef struct typeName##Item {\
    unsigned key;\
    dataType data;\
  } typeName##Item;\
  \
  typedef struct typeName {\
    size_t reservedCount;\
    size_t itemCount;\
    typeName##Item* item;\
  } typeName;

  #define DECLARE_UINT_KEYARRAY_CREATE( funcName, listType )\
  listType* funcName( size_t reserveCount ) {\
    listType* newKeyArray = NULL;\
    \
    newKeyArray = (listType*)calloc(1, sizeof(listType));\
    if( newKeyArray == NULL ) {\
      goto ReturnError;\
    }\
    \
    if( reserveCount ) {\
      newKeyArray->item =\
        (listType##Item*)calloc(reserveCount, sizeof(listType##Item));\
      if( newKeyArray->item == NULL ) {\
        goto ReturnError;\
      }\
      \
      newKeyArray->reservedCount = reserveCount;\
    }\
    return newKeyArray;\
    \
  ReturnError:\
    if( newKeyArray ) {\
      if( newKeyArray->item ) {\
        free( newKeyArray->item );\
        newKeyArray->item = NULL;\
      }\
      free( newKeyArray );\
      newKeyArray = NULL;\
    }\
    return NULL;\
  }

  #define DECLARE_UINT_KEYARRAY_FREE( funcName, listType, freeDataFunc )\
  void funcName( listType** keyList ) {\
    size_t index;\
    size_t itemCount;\
    \
    if( keyList && (*keyList) ) {\
      itemCount = (*keyList)->itemCount;\
      for( index = 0; index < itemCount; index++ ) {\
        freeDataFunc( &((*keyList)->item[index].data) );\
      }\
      \
      free( (*keyList) );\
      (*keyList) = NULL;\
    }\
  }

  #define DECLARE_UINT_KEYARRAY_INSERT( funcName, listType, dataType )\
  int funcName( listType* keyList,\
      unsigned key, dataType* data ) {\
    unsigned leftIndex;\
    unsigned insertIndex;\
    unsigned rightIndex;\
    unsigned prevCount;\
    unsigned reservedCount;\
    unsigned itemCount;\
    listType##Item* item;\
    \
    if( !(keyList && data) ) {\
      return 0;\
    }\
    \
    /* Grow list, if necessary */\
    reservedCount = keyList->reservedCount;\
    itemCount = keyList->itemCount;\
    item = keyList->item;\
    \
    if( itemCount == reservedCount ) {\
      prevCount = reservedCount;\
      reservedCount += 8;\
      if( prevCount > reservedCount ) {\
        return 0;\
      }\
      \
      item = realloc(item, reservedCount * sizeof(listType##Item));\
      if( item == NULL ) {\
        return 0;\
      }\
      keyList->reservedCount = reservedCount;\
      keyList->item = item;\
    }\
    \
    /* Search for insert position */\
    leftIndex = 0;\
    rightIndex = itemCount;\
    insertIndex = itemCount / 2;\
    \
    while( leftIndex < rightIndex ) {\
      if( item[insertIndex].key == key ) {\
        return 0;\
      }\
      \
      if( item[insertIndex].key > key ) {\
        rightIndex = insertIndex;\
      } else {\
        leftIndex = insertIndex + 1;\
      }\
      \
      insertIndex = (leftIndex + rightIndex) / 2;\
    }\
    \
    /* Move data past insertion point up, if necessary */\
    memmove( &(item[insertIndex + 1]), &(item[insertIndex]),\
        (itemCount - insertIndex) * sizeof(listType##Item) );\
    \
    /* Insert item */\
    item[insertIndex].key = key;\
    if( data ) {\
      memcpy( &(item[insertIndex].data), data, sizeof(dataType) );\
    }\
    \
    keyList->itemCount++;\
    \
    return 1;\
  }

  #define DECLARE_UINT_KEYARRAY_REMOVE( funcName, listType, freeDataFunc )\
  void funcName( listType* keyList, unsigned key ) {\
    unsigned leftIndex;\
    unsigned rightIndex;\
    unsigned removeIndex;\
    unsigned reservedCount;\
    unsigned itemCount;\
    listType##Item* item;\
    \
    if( !(keyList && keyList->item) ) {\
      return;\
    }\
    \
    reservedCount = keyList->reservedCount;\
    itemCount = keyList->itemCount;\
    item = keyList->item;\
    \
    /* Search for insert position */\
    leftIndex = 0;\
    rightIndex = itemCount;\
    removeIndex = itemCount / 2;\
    \
    while( leftIndex < rightIndex ) {\
      if( item[removeIndex].key == key ) {\
        freeDataFunc( &(item[removeIndex].data) );\
        \
        if( itemCount ) {\
          itemCount--;\
          \
          memcpy( &(item[removeIndex]), &(item[removeIndex + 1]),\
            (itemCount - removeIndex) * sizeof(listType##Item) );\
          \
          keyList->itemCount = itemCount;\
        }\
        \
        memset( &(item[itemCount]), 0, sizeof(listType##Item) );\
        \
        return;\
      }\
      \
      if( item[removeIndex].key > key ) {\
        rightIndex = removeIndex;\
      } else {\
        leftIndex = removeIndex + 1;\
      }\
      \
      removeIndex = (leftIndex + rightIndex) / 2;\
    }\
  }

  #define DECLARE_UINT_KEYARRAY_RETRIEVE( funcName, listType, dataType )\
  int funcName( listType* keyList, unsigned key,\
      dataType* destData ) {\
    unsigned leftIndex;\
    unsigned rightIndex;\
    unsigned retrieveIndex;\
    unsigned reservedCount;\
    unsigned itemCount;\
    listType##Item* item;\
    \
    if( !(keyList && keyList->item && destData) ) {\
      return 0;\
    }\
    \
    reservedCount = keyList->reservedCount;\
    itemCount = keyList->itemCount;\
    item = keyList->item;\
    \
    /* Search for insert position */\
    leftIndex = 0;\
    rightIndex = itemCount;\
    retrieveIndex = itemCount / 2;\
    \
    while( leftIndex < rightIndex ) {\
      if( item[retrieveIndex].key == key ) {\
        memcpy( destData, &(item[retrieveIndex].data), sizeof(dataType) );\
        return 1;\
      }\
      \
      if( item[retrieveIndex].key > key ) {\
        rightIndex = retrieveIndex;\
      } else {\
        leftIndex = retrieveIndex + 1;\
      }\
      \
      retrieveIndex = (leftIndex + rightIndex) / 2;\
    }\
    \
    return 0;\
  }

  #define DECLARE_UINT_KEYARRAY_RELEASEUNUSED( funcName, listType )\
  void funcName( listType* keyList ) {\
    listType##Item* item;\
    \
    if( keyList == NULL ) {\
      return;\
    }\
    \
    if( keyList->item && keyList->itemCount ) {\
      /* Resize to remove reserved space */\
      item = realloc(keyList->item,\
        keyList->itemCount * sizeof(listType##Item));\
      if( item ) {\
        keyList->item = item;\
        keyList->reservedCount = keyList->itemCount;\
      }\
    } else {\
      /* Deallocate */\
      keyList->reservedCount = 0;\
      keyList->itemCount = 0;\
      if( keyList->item ) {\
        free( keyList->item );\
        keyList->item = NULL;\
      }\
    }\
  }

  #define DECLARE_UINT_KEYARRAY_COPY( funcName, listType, dataType,\
      copyDataFunc, freeDataFunc )\
  listType* funcName( listType* sourceList ) {\
    listType* newCopy = NULL;\
    listType##Item* sourceItem = NULL;\
    size_t reservedCount = 0;\
    size_t itemCount = 0;\
    size_t index;\
    \
    if( sourceList == NULL ) {\
      return NULL;\
    }\
    \
    /* Attempt to allocate list object */\
    newCopy = calloc(1, sizeof(listType));\
    if( newCopy == NULL ) {\
      goto ReturnError;\
    }\
    \
    /* Initialize important variables */\
    reservedCount = sourceList->reservedCount;\
    itemCount = sourceList->itemCount;\
    sourceItem = sourceList->item;\
    \
    /* A list with no items is valid */\
    if( !(reservedCount && itemCount && sourceItem) ) {\
      return newCopy;\
    }\
    \
    /* Copy data, then copy the Uint keys */\
    newCopy->item = malloc(reservedCount * sizeof(listType##Item));\
    if( newCopy->item == NULL ) {\
      goto ReturnError;\
    }\
    \
    for( index = 0; index < itemCount; index++ ) {\
      copyDataFunc( &(newCopy->item[index].data),\
          &(sourceItem[index].data) );\
      \
      newCopy->item[index].key = sourceItem[index].key;\
    }\
    \
    newCopy->reservedCount = reservedCount;\
    newCopy->itemCount = itemCount;\
    \
    return newCopy;\
    \
  ReturnError:\
    if( newCopy == NULL ) {\
      return NULL;\
    }\
    \
    if( newCopy->item ) {\
      for( index = 0; index < itemCount; index++ ) {\
        freeDataFunc( &(newCopy->item[index].data) );\
      }\
    }\
    \
    free( newCopy );\
    newCopy = NULL;\
    \
    return NULL;\
  }

#endif
