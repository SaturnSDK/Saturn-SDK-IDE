##---------------------------------------------------------------------------
## $RCSfile$
## $Source$
## $Revision$
## $Date$
##---------------------------------------------------------------------------
## Author:      Jorgen Bodde
## Copyright:   (c) Jorgen Bodde
## License:     wxWidgets License
##---------------------------------------------------------------------------

# Add files to library treemultictrl

ADD_LIBRARY(treemultictrl TreeMultiItemBase.cpp TreeMultiItemNode.cpp TreeMultiItemWindow.cpp
                          TreeMultiItemRoot.cpp wxTreeMultiCtrl.cpp 
##                          TreeMultiXmlMapper.cpp TmcWizardObjects.cpp TmcWizardWindows.cpp
                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/wxTreeMultiCtrl.h
                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TreeMultiItemBase.h 
                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TreeMultiItemNode.h
                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TreeMultiItemRoot.h ) 
##                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TreeMultiItemWindow.h 
##                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TreeMultiXmlMapper.h
##                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TmcWizardObjects.h
##                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TmcWizardWindows.h )

IF(LINUX)
  ADD_DEFINITIONS( -DLINUX=1 )
ENDIF(LINUX)

# if the checkbox view functionality is not desired, we 
# won't include it in the compile process. This way the 
# control can remain small if new features are applied.

IF(USE_CHECKBOXVIEW)
  ADD_DEFINITIONS( -DCHECKBOXVIEW=1 )
ELSE(USE_CHECKBOXVIEW)
  ADD_DEFINITIONS( -DCHECKBOXVIEW=0)
ENDIF(USE_CHECKBOXVIEW)

# if the XML mapper is not desired, it is 
# ifdef'ed out of the source so no compile
# errors will occur

##IF(USE_XMLMAPPER)
##	ADD_DEFINITIONS( -DwxUSE_TMC_XMLMAPPER=1 )
##ENDIF(USE_XMLMAPPER)