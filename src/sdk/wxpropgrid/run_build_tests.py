#
# Builds and runs tests for wxPropertyGrid
#

import sys, os, os.path

from shutil import copyfile


run_list = [
('prepare_vc6.bat', '2.6.4',  r'D:\lib\wxWidgets-2.6.4', 'WX_UNICODE=1 WX_DEBUG=0'),
# Failed on command line, succesfull in IDE. What's going on?
#('prepare_vc6.bat', '2.8-SVN',  r'D:\lib\wxWidgets_SVN_28', 'WX_UNICODE=1 WX_DEBUG=1'),
('prepare_vc8.bat', '2.9.0',  r'D:\lib\wxWidgets_SVN', 'WX_UNICODE=1 WX_DEBUG=1'),
]


def main():
    cwd = os.getcwd()
    comp_src = cwd

    only_wxversion = None
    resume_from_wxversion = None
    resumed = True

    for arg in sys.argv[1:]:
        if arg.startswith('--only-wxversion'):
            only_wxversion = arg.split('=',1)[1].strip()
        elif arg.startswith('--resume-from-wxversion'):
            resume_from_wxversion = arg.split('=',1)[1].strip()
        else:
            resume_from_wxversion = arg

    if resume_from_wxversion:
        resumed = False

    # Check dirs
    for prepare_script, wxversion, wxdir, build_opts in run_list:
        if not os.path.isdir(wxdir):
            print '-- No such path: %s'%wxdir
            return

    for prepare_script, wxversion, wxdir, build_opts in run_list:

        if only_wxversion and not (only_wxversion == wxversion):
            continue

        if not resumed:
            if resume_from_wxversion and resume_from_wxversion == wxversion:
                resumed = True
            else:
                continue

        print ''
        print 'Building for \'wxWidgets %s - %s\''%(wxversion, build_opts)

        if wxversion.startswith('2.6'):
            build_subdir = 'build_wx26'
        elif wxversion.startswith('2.8'):
            build_subdir = 'build'
        else:
            build_subdir = 'build_wx29'

        build_path = os.path.join(comp_src, build_subdir)
        sample_path = os.path.join(comp_src, 'samples')

        os.chdir(build_path)

        #res = os.system(prepare_script)
        #if res:
        #    print '-- Prepare script(%s) failed --'%prepare_script
        #    return

        #os.environ['WXWIN'] = wxdir

        build_clean_cmdline = 'nmake -f makefile.vc clean %s'%(build_opts)
        build_cmdline = 'nmake -f makefile.vc %s'%(build_opts)

        temp_builder_fn = '__temp_builder__.bat'

        f = file(temp_builder_fn,'wt')
        f.write('@echo off\n')
        f.write('call %s\n'%prepare_script)
        f.write('set WXWIN=%s\n'%wxdir)
        f.write('%s\n'%build_clean_cmdline)
        f.write('%s\n'%build_cmdline)
        f.close()

        res = os.system(temp_builder_fn)
        os.remove(temp_builder_fn)
        if res:
            print '-- \'%s\' failed --'%build_cmdline
            return

        os.chdir(sample_path)

        # Copy sample app for further testing
        copyfile('propgridsample.exe', 'propgridsample (%s %s).exe'%(wxversion, build_opts))
        res = os.system('propgridsample.exe --run-tests')

if __name__ == '__main__':
    main()
