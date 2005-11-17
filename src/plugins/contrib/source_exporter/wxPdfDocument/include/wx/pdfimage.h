///////////////////////////////////////////////////////////////////////////////
// Name:        pdfimage.h
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-08-11
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfimage.h Interface of the wxPdfImage class

#ifndef _PDFIMAGE_H_
#define _PDFIMAGE_H_

//#if defined(__GNUG__) && !defined(__APPLE__)
//    #pragma interface "pdfimage.h"
//#endif

#include <wx/filesys.h>

#include "wx/pdfdocdef.h"

// Forward declarations

class WXDLLIMPEXP_PDFDOC wxPdfDocument;

/// Class representing image objects. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfImage
{
public:
  /// Constructor
  wxPdfImage(wxPdfDocument* document, int index, const wxString& name, const wxString& type);

  /// Destructor
  virtual ~wxPdfImage();

  /// Set image index
  void SetIndex(int index) { m_index = index; }

  /// Get image index
  int  GetIndex() { return m_index; }

  /// Set object index
  void SetObjIndex(int n) { m_n = n; }

  /// Get object index
  int  GetObjIndex() { return m_n; }

  /// Set image type
  void SetType(const wxString& type) { m_type = type; }

  /// Get image type
  wxString GetType() { return m_type; }

  /// Check whether image has to be handled as a form object
  bool IsFormObject() { return m_isFormObj; }

  /// Set image file name
  void SetName(const wxString& name) { m_name = name; }

  /// Get image file name
  wxString GetName() { return m_name; }

  /// Get image width in pixels
  int GetWidth() { return m_width; }

  /// Get image height in pixels
  int GetHeight() { return m_height; }

  /// Get image offset in X direction
  int GetX() { return m_x; }

  /// Get image offset in Y direction
  int GetY() { return m_y; }

  /// Get color space
  wxString GetColorSpace() { return m_cs; }

  /// Get bits per component
  int GetBitsPerComponent() { return m_bpc; }

  /// Get compression filter specification
  wxString GetF() { return m_f; }

  /// Get additional PDF parameter
  wxString GetParms() { return m_parms; }

  /// Get palette size
  int GetPaletteSize() { return m_palSize; }

  /// Get palette data
  char* GetPalette() { return m_pal; }

  /// Get transparency size
  int GetTransparencySize() { return m_trnsSize; }

  /// Get transparency data
  char* GetTransparency() { return m_trns; }

  /// Get image data size
  int GetDataSize() { return m_dataSize; }

  /// Get image data
  char* GetData() { return m_data; }

  /// Parse image file
  bool Parse();

protected:
  /// Extract info from a JPEG file
  bool ParseJPG(wxInputStream* imageStream);

  /// Extract info from a PNG file
  bool ParsePNG(wxInputStream* imageStream);

  /// Extract info from a GIF file
  bool ParseGIF(wxInputStream* imageStream);

  /// Extract info from a WMF file
  bool ParseWMF(wxInputStream* imageStream);

  /// Read a 4-byte integer from file (big endian)
  int ReadIntBE(wxInputStream* imageStream);

  /// Read a 4-byte integer from file (little endian)
  int ReadIntLE(wxInputStream* imageStream);
  
  /// Read a 4-byte unsigned integer from file (big endian)
  unsigned int ReadUIntBE(wxInputStream* imageStream);

  /// Read a 4-byte unsigned integer from file (little endian)
  unsigned int ReadUIntLE(wxInputStream* imageStream);
  
  /// Read a 2-byte integer from file (big endian)
  short ReadShortBE(wxInputStream* imageStream);

  /// Read a 2-byte integer from file (little endian)
  short ReadShortLE(wxInputStream* imageStream);

  /// Read a unsigned 2-byte integer from file (big endian)
  unsigned short ReadUShortBE(wxInputStream* imageStream);

  /// Read a unsigned 2-byte integer from file (little endian)
  unsigned short ReadUShortLE(wxInputStream* imageStream);

  /// Get the file system for accessing image files
  static wxFileSystem* GetFileSystem();

protected:
  wxPdfDocument* m_document;  ///< Document this image belongs to
  int            m_index;     ///< Index number of this image
  int            m_n;         ///< Image object index
  wxString       m_type;      ///< Image type
  wxString       m_name;      ///< Image name

  int            m_width;     ///< Image width in pixels
  int            m_height;    ///< Image height in pixels
  wxString       m_cs;        ///< Colorspace
  char           m_bpc;       ///< Bits per color
  wxString       m_f;         ///< Compression method
  wxString       m_parms;     ///< Additional PDF parameters
  int            m_palSize;   ///< Size of palette
  char*          m_pal;       ///< Palette data
  int            m_trnsSize;  ///< Transparency color size
  char*          m_trns;      ///< Transparency color data
  int            m_dataSize;  ///< Image data size
  char*          m_data;      ///< Image data

  bool           m_isFormObj; ///< Flag whether image must be treated as form object
  int            m_x;         ///< Offset in X direction
  int            m_y;         ///< Offset in Y direction

  static wxFileSystem* ms_fileSystem; ///< File system for accessing image files
};

#endif
