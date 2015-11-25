# QmitkPathTrackView

message(STATUS "processing QmitkPathTrackView.cmake")
message(STATUS "APPMOD_CPP before: ${APPMOD_CPP}")

set( APPMOD_H ${APPMOD_H} QmitkPathTrackView.h )
set( APPMOD_MOC_H ${APPMOD_MOC_H} QmitkPathTrackView.h )
set( APPMOD_CPP ${APPMOD_CPP} QmitkPathTrackView.cpp )
set(APPMOD_FORMS ${APPMOD_FORMS} QmitkPathTrackViewControls.ui)
message(STATUS "APPMOD_CPP after: ${APPMOD_CPP}")

