# The project name must correspond to the directory name of your plug-in
# and must not contain periods.
project(org_mitk_csi_gui_qt_pathtrack)

mitk_create_plugin(
  EXPORT_DIRECTIVE PATHTRACK_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkQtWidgetsExt MitkFiberTracking
)
