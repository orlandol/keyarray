#include <stdio.h>

#include "keyarray.h"

/*
 *  File: strkey1.c
 *  Status: Complete
 *
 *  Simple String Key Array Example: Color to RGB Hex list
 *    by Orlando Llanes
 *
 *  Sample data source: Wikipedia via W3C (details below)
 *
 *  https://github.com/orlandol/keyarray
 */

/*
 * Sample data declarations
 */

  typedef struct SampleData {
    char* name;
    unsigned rgbValue;
  } SampleData;

  // sampleData declared at end of file for clarity
  extern const SampleData sampleData[148];
  const size_t sampleCount = sizeof(sampleData) / sizeof(sampleData[0]);

/*
 *  Color to RGB declarations
 */

  DECLARE_STRING_KEYARRAY_TYPES( ColorList, unsigned )

  void FreeRGBItem( unsigned* data ) {
  }

  DECLARE_STRING_KEYARRAY_CREATE( CreateColorList, ColorList )
  DECLARE_STRING_KEYARRAY_FREE( ReleaseColorList, ColorList, FreeRGBItem )

  DECLARE_STRING_KEYARRAY_INSERT( InsertColor, ColorList, unsigned )
  DECLARE_STRING_KEYARRAY_RETRIEVE( RetrieveRGB, ColorList, unsigned )

/*
 *  Main program
 */
int main( int argc, char* argv[] ) {
  ColorList* svg1Colors = NULL;
  size_t sampleIndex = 0;
  char* sampleName = NULL;
  unsigned sampleRGBValue = 0;
  unsigned rgbValue = 0;

  // Create color list
  svg1Colors = CreateColorList(0);
  if( svg1Colors == NULL ) {
    printf( "Error allocating color list\n" );
    return 1;
  }

  // Insert sample data into list
  printf( "\nInserting sample data into list...\n" );
  for( sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++ ) {
    sampleName = sampleData[sampleIndex].name;
    sampleRGBValue = sampleData[sampleIndex].rgbValue;

    // Attempt to add item to the list
    if( InsertColor(svg1Colors, sampleName, &sampleRGBValue) == 0 ) {

      // Check if item is duplicated
      if( RetrieveRGB(svg1Colors, sampleName, &rgbValue) ) {
        // If so, report duplicated item
        printf( "  Item '%s' is already in the list.\n", sampleName );
      } else {
        // Otherwise, report general error
        printf( "  Error inserting sample item: ('%s', %08X).\n",
            sampleName, sampleRGBValue );
      }
      break;
    }
  }
  printf( "Done.\n" );

  // Retrieve select items
  printf( "\nRetrieving select items...\n" );
  if( RetrieveRGB(svg1Colors, "aliceblue", &rgbValue) ) { // First item
    printf( "  Found '%s': RGB value = #%08X\n", "aliceblue", rgbValue );
  } else {
    printf( "  Item '%s' not found\n", "aliceblue" );
  }

  if( RetrieveRGB(svg1Colors, "crimson", &rgbValue) ) {
    printf( "  Found '%s': RGB value = #%08X\n", "crimson", rgbValue );
  } else {
    printf( "  Item '%s' not found\n", "crimson" );
  }

  if( RetrieveRGB(svg1Colors, "sienna", &rgbValue) ) {
    printf( "  Found '%s': RGB value = #%08X\n", "sienna", rgbValue );
  } else {
    printf( "  Item '%s' not found\n", "sienna" );
  }

  if( RetrieveRGB(svg1Colors, "olive", &rgbValue) ) {
    printf( "  Found '%s': RGB value = #%08X\n", "olive", rgbValue );
  } else {
    printf( "  Item '%s' not found\n", "olive" );
  }

  if( RetrieveRGB(svg1Colors, "yellowgreen", &rgbValue) ) { // Last item
    printf( "  Found '%s': RGB value = #%08X\n", "yellowgreen", rgbValue );
  } else {
    printf( "  Item '%s' not found\n", "yellowgreen" );
  }

  if( RetrieveRGB(svg1Colors, "cornsilk", &rgbValue) ) { // Duplicated item
    printf( "  Found '%s': RGB value = #%08X\n", "cornsilk", rgbValue );
  } else {
    printf( "  Item '%s' not found\n", "cornsilk" );
  }

  if( RetrieveRGB(svg1Colors, "cherryred", &rgbValue) ) { // Not in list
    printf( "  Found '%s': RGB value = #%08X\n", "cherryred", rgbValue );
  } else {
    printf( "  Item '%s' not found\n", "cherryred" );
  }
  printf( "Done.\n" );

  // Release resources
  ReleaseColorList( &svg1Colors );

  return 0;
}

/*
 *
 *  Sample Data: SVG 1.0 color name (aka X11 Color Names)
 *
 *  Source: Wikipedia via W3C
 *  https://en.wikipedia.org/wiki/X11_color_names
 *  https://www.w3.org/TR/2018/REC-css-color-3-20180619/#svg-color
 *
 *  List order randomized through BrowserLing
 *  https://www.browserling.com/tools/random-lines
 *
 */
  const SampleData sampleData[148] = {
    "olivedrab", 0x6B8E23,
    "floralwhite", 0xFFFAF0,
    "chocolate", 0xD2691E,
    "paleturquoise", 0xAFEEEE,
    "khaki", 0xF0E68C,
    "royalblue", 0x4169E1,
    "darkslategrey", 0x2F4F4F,
    "darkturquoise", 0x00CED1,
    "sandybrown", 0xF4A460,
    "lightgray", 0xD3D3D3,
    "yellow", 0xFFFF00,
    "mediumorchid", 0xBA55D3,
    "seashell", 0xFFF5EE,
    "gold", 0xFFD700,
    "indianred", 0xCD5C5C,
    "lightgoldenrodyellow", 0xFAFAD2,
    "mediumblue", 0x0000CD,
    "deepskyblue", 0x00BFFF,
    "lime", 0x00FF00,
    "beige", 0xF5F5DC,
    "mediumaquamarine", 0x66CDAA,
    "lightgrey", 0xD3D3D3,
    "lightsalmon", 0xFFA07A,
    "green", 0x008000,
    "red", 0xFF0000,
    "blueviolet", 0x8A2BE2,
    "snow", 0xFFFAFA,
    "navajowhite", 0xFFDEAD,
    "rosybrown", 0xBC8F8F,
    "darksalmon", 0xE9967A,
    "teal", 0x008080,
    "yellowgreen", 0x9ACD32,
    "darkblue", 0x00008B,
    "goldenrod", 0xDAA520,
    "moccasin", 0xFFE4B5,
    "honeydew", 0xF0FFF0,
    "greenyellow", 0xADFF2F,
    "darkkhaki", 0xBDB76B,
    "firebrick", 0xB22222,
    "tan", 0xD2B48C,
    "darkmagenta", 0x8B008B,
    "orange", 0xFFA500,
    "mediumspringgreen", 0x00FA9A,
    "aquamarine", 0x7FFFD4,
    "palegoldenrod", 0xEEE8AA,
    "steelblue", 0x4682B4,
    "indigo", 0x4B0082,
    "grey", 0x808080,
    "slategrey", 0x708090,
    "lightseagreen", 0x20B2AA,
    "orchid", 0xDA70D6,
    "whitesmoke", 0xF5F5F5,
    "hotpink", 0xFF69B4,
    "darkseagreen", 0x8FBC8F,
    "dodgerblue", 0x1E90FF,
    "azure", 0xF0FFFF,
    "lightslategrey", 0x778899,
    "aqua", 0x00FFFF,
    "lavender", 0xE6E6FA,
    "thistle", 0xD8BFD8,
    "darkred", 0x8B0000,
    "turquoise", 0x40E0D0,
    "darkviolet", 0x9400D3,
    "maroon", 0x800000,
    "silver", 0xC0C0C0,
    "powderblue", 0xB0E0E6,
    "slateblue", 0x6A5ACD,
    "palegreen", 0x98FB98,
    "black", 0x000000,
    "mediumvioletred", 0xC71585,
    "lightyellow", 0xFFFFE0,
    "lawngreen", 0x7CFC00,
    "peachpuff", 0xFFDAB9,
    "saddlebrown", 0x8B4513,
    "darkgreen", 0x006400,
    "skyblue", 0x87CEEB,
    "lemonchiffon", 0xFFFACD,
    "orangered", 0xFF4500,
    "bisque", 0xFFE4C4,
    "linen", 0xFAF0E6,
    "wheat", 0xF5DEB3,
    "ivory", 0xFFFFF0,
    "palevioletred", 0xDB7093,
    "lightblue", 0xADD8E6,
    "slategray", 0x708090,
    "plum", 0xDDA0DD,
    "salmon", 0xFA8072,
    "antiquewhite", 0xFAEBD7,
    "darkslategray", 0x2F4F4F,
    "mistyrose", 0xFFE4E1,
    "darkorchid", 0x9932CC,
    "crimson", 0xDC143C,
    "violet", 0xEE82EE,
    "deeppink", 0xFF1493,
    "papayawhip", 0xFFEFD5,
    "pink", 0xFFC0CB,
    "darkolivegreen", 0x556B2F,
    "oldlace", 0xFDF5E6,
    "aliceblue", 0xF0F8FF,
    "lightgreen", 0x90EE90,
    "cornflowerblue", 0x6495ED,
    "magenta", 0xFF00FF,
    "seagreen", 0x2E8B57,
    "darkslateblue", 0x483D8B,
    "mediumslateblue", 0x7B68EE,
    "dimgray", 0x696969,
    "purple", 0x800080,
    "tomato", 0xFF6347,
    "lightslategray", 0x778899,
    "fuchsia", 0xFF00FF,
    "coral", 0xFF7F50,
    "dimgrey", 0x696969,
    "darkgoldenrod", 0xB8860B,
    "limegreen", 0x32CD32,
    "ghostwhite", 0xF8F8FF,
    "darkorange", 0xFF8C00,
    "chartreuse", 0x7FFF00,
    "lightpink", 0xFFB6C1,
    "gainsboro", 0xDCDCDC,
    "blue", 0x0000FF,
    "forestgreen", 0x228B22,
    "mediumturquoise", 0x48D1CC,
    "brown", 0xA52A2A,
    "darkcyan", 0x008B8B,
    "sienna", 0xA0522D,
    "peru", 0xCD853F,
    "cyan", 0x00FFFF,
    "lavenderblush", 0xFFF0F5,
    "mediumpurple", 0x9370DB,
    "lightcoral", 0xF08080,
    "mintcream", 0xF5FFFA,
    "lightcyan", 0xE0FFFF,
    "lightsteelblue", 0xB0C4DE,
    "midnightblue", 0x191970,
    "blanchedalmond", 0xFFEBCD,
    "navy", 0x000080,
    "gray", 0x808080,
    "olive", 0x808000,
    "burlywood", 0xDEB887,
    "mediumseagreen", 0x3CB371,
    "lightskyblue", 0x7CEFA,
    "white", 0xFFFFFF,
    "darkgrey", 0xA9A9A9,
    "springgreen", 0x00FF7F,
    "cadetblue", 0x5F9EA0,
    "darkgray", 0xA9A9A9,
    "cornsilk", 0xFFF8DC,
    "cornsilk", 0x112233 // Intentional duplicate item/invalid value
  };
