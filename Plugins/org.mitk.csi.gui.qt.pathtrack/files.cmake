set(SRC_CPP_FILES

)

set(INTERNAL_CPP_FILES
  QmitkPathTrackView.cpp
  mitkPluginActivator.cpp
)

set(UI_FILES
  src/internal/QmitkPathTrackViewControls.ui
)

set(MOC_H_FILES
  src/internal/mitkPluginActivator.h
  src/internal/QmitkPathTrackView.h
)

set(CACHED_RESOURCE_FILES
  resources/pathtrack.png
  plugin.xml
)

set(QRC_FILES
  resources/QmitkPathTrackView.qrc
)

foreach(file ${SRC_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})

