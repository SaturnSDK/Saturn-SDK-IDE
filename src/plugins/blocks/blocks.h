#ifndef BLOCKS_H
#define BLOCKS_H

#include <cbplugin.h>
#include <settings.h>
#include "configurationpanel.h"
#include <wx/filename.h>

class wxWindow;

class Blocks : public cbToolPlugin
{
    wxString lastError;

	public:
		Blocks();
		~Blocks();
		int GetConfigurationPriority() const{ return  25; }
		int GetConfigurationGroup() const{ return  cgUnknown; }
		cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
		int Execute();
		void OnAttach();
		void OnRelease(bool appShutDown);

		bool InstallBlock(const wxString& filename);
		bool InstallTBZ(const wxString& filename);
		bool InstallTGZ(const wxString& filename);
		bool UnTar(const wxFile& file){return true;};
};

CB_DECLARE_PLUGIN();


class BlocksConfigDlg : public cbConfigurationPanel
{
    Blocks* plugin;

	public:
		BlocksConfigDlg(wxWindow* parent, Blocks* plug);
		virtual ~BlocksConfigDlg(){};

        virtual wxString GetTitle() const { return _T("Blocks - organising blocks"); }
        virtual wxString GetBitmapBaseName() const { return _T("blocks"); }
        virtual void OnApply(){ SaveSettings(); }
        virtual void OnCancel(){}

        void LoadSettings();
        void SaveSettings();
};



class BlocksSelectDlg : public wxDialog
{
	public:
		BlocksSelectDlg(wxWindow* parent, Blocks* plug);


};

class TempFile : public wxFile
  {
  public:
    wxString name;

    TempFile()
    {
    name = wxFileName::CreateTempFileName(wxEmptyString, this);
    };

    ~TempFile()
    {
      Close();
      if(!name.IsEmpty())
        ::wxRemoveFile(name);
    };
  };



#include "md5.h"

class MD5
{
    md5_byte_t digest[16];
    char hex[36];               // 32 + \0 + padding

    void ToHex()
    {
        assert(sizeof(int) == 4); // otherwise we get in trouble...

        memset(hex, 0, sizeof(hex));

        char buffer[2];

        char *hgh = (char *) hex;
        char *bin_p = (char *) digest;
        char *hex_p = (char *) hex;


        for(unsigned int i= 0; i< 16; ++i)
        {
            itoa((unsigned int) bin_p[i], buffer, 16);
            *hex_p++ = buffer[0];
            *hex_p++ = buffer[1];
    	}

	};

    void ParseFile(wxFile& f)
    {
        if(f.IsOpened())
        {
            memcpy(hex, "d41d8cd98f00b204e9800998ecf8427e", 32); // empty MD5
            return;
        }

        md5_byte_t buf[4096];
        int len;

        md5_state_t state;

        md5_init(&state);

        while( (len = f.Read(buf, sizeof(buf)) ) != wxInvalidOffset)
            md5_append(&state, buf, len);

        md5_finish(&state, digest);
        ToHex();
    }

public:

    MD5(wxFile& f)
    {
        ParseFile(f);
    };

    MD5(const wxString& filename)
    {
        wxFile f(filename, wxFile::read);
        ParseFile(f);
    };

    MD5(const char* ptr, size_t len)
    {
        md5_state_t state;

        md5_init(&state);
        md5_append(&state, (const md5_byte_t *) ptr, len);
        md5_finish(&state, digest);
    };

    const char* Hash() const
    {
        return hex;
    };

    operator wxString()
    {
        ToHex();

        return wxString(hex, wxConvUTF8, 32);
    };
};

#endif // BLOCKS_H

