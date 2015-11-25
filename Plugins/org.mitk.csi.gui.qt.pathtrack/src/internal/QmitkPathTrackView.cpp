/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

// Qmitk related includes
#include "QmitkPathTrackView.h"
#include "ui_QmitkPathTrackViewControls.h"

#include <berryIWorkbenchWindow.h>
#include <berryISelectionService.h>
#include <QInputDialog>
#include <QLineEdit>
#include <mitkProperties.h>
#include <QmitkPointListWidget.h>
#include <mitkDataNodeObject.h>
#include <mitkDataNodeSelection.h>
#include <mitkFiberBundle.h>

#include <vtkPolyLine.h>
#include <vtkCellArray.h>

QmitkPathTrackView::QmitkPathTrackView( QObject* /*parent*/ )
: m_Controls(0)
{

}

QmitkPathTrackView::~QmitkPathTrackView()
{
}

void QmitkPathTrackView::CreateQtPartControl( QWidget *parent )
{
  m_Controls = new Ui::QmitkPathTrackViewControls;
  m_Controls->setupUi(parent);
  m_Controls->m_PbAddPointSet->connect( m_Controls->m_PbAddPointSet, SIGNAL( clicked() )
    , this, SLOT( OnAddPointSetClicked() ) );
  m_Controls->m_PbConnectPoints->connect( m_Controls->m_PbConnectPoints, SIGNAL ( clicked() )
    , this, SLOT(DoGenerateFBfromPoints() ) );
}

void QmitkPathTrackView::Activated()
{
  // emulate datamanager selection
  std::vector<mitk::DataNode*> selection = this->GetDataManagerSelection();
  this->OnSelectionChanged( selection );
}

void QmitkPathTrackView::Deactivated()
{
  // emulate empty selection
  std::vector<mitk::DataNode*> selection;
  this->OnSelectionChanged( selection );
  m_Controls->m_PointListWidget->DeactivateInteractor(true);
}

void QmitkPathTrackView::OnAddPointSetClicked()
{
  //Ask for the name of the point set
  bool ok = false;
  QString name = QInputDialog::getText( QApplication::activeWindow()
    , "Add point set...", "Enter name for the new point set", QLineEdit::Normal, "PathTrack", &ok );
  if ( ! ok || name.isEmpty() )
    return;

  //
  //Create a new empty pointset
  //
  mitk::PointSet::Pointer pointSet = mitk::PointSet::New();
  //
  // Create a new data tree node
  //
  mitk::DataNode::Pointer pointSetNode = mitk::DataNode::New();
  //
  // fill the data tree node with the appropriate information
  //
  pointSetNode->SetData( pointSet );
  pointSetNode->SetProperty( "name", mitk::StringProperty::New( name.toStdString() ) );
  pointSetNode->SetProperty( "opacity", mitk::FloatProperty::New( 1 ) );
  pointSetNode->SetColor( 1.0, 1.0, 0.0 );
  //
  // add the node to the ds
  //
  this->GetDefaultDataStorage()->Add(pointSetNode);

  // make new selection and emulate selection for this
  std::vector<mitk::DataNode*> selection;
  selection.push_back( pointSetNode );
  this->FireNodesSelected( selection );
  this->OnSelectionChanged( selection );
}

void QmitkPathTrackView::DoGenerateFBfromPoints()
{
    /*
     * A valid polydata consists of a point set, vtkPoints and lines, vtkPolyLine.
     * The polyline data structure hosted all related point IDs from the point set.
     */
    mitk::PointSet* pointSet = dynamic_cast<mitk::PointSet*> ( m_SelectedPointSetNode->GetData() );

    vtkSmartPointer<vtkPolyLine> polyline = vtkSmartPointer<vtkPolyLine>::New();

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
      lines->InsertNextCell(pointSet->GetSize());

    for (int i=0;i<pointSet->GetSize();i++)
    {
        mitk::Point3D mp = pointSet->GetPoint(i);
        double p[3];
        p[0] = mp[0];
        p[1] = mp[1];
        p[2] = mp[2];

        MITK_INFO << "transformed point" << p;
        points->InsertNextPoint(p);
        lines->InsertCellPoint(i);

    }

    vtkSmartPointer<vtkPolyData> fiberPolyData = vtkSmartPointer<vtkPolyData>::New();
    fiberPolyData->SetPoints(points);
    fiberPolyData->SetLines(lines);
    mitk::FiberBundle::Pointer mitkFiberBundle = mitk::FiberBundle::New(fiberPolyData);

    mitk::DataNode::Pointer fbNode = mitk::DataNode::New();
    fbNode->SetData(mitkFiberBundle);
    fbNode->SetName("Bullet Path");
    fbNode->SetVisibility(true);
    this->GetDataStorage()->Add(fbNode);
}



void QmitkPathTrackView::OnSelectionChanged(std::vector<mitk::DataNode*> nodes)
{
  mitk::DataNode* selectedNode = 0;
  if(nodes.size() > 0)
    selectedNode = nodes.front();

  mitk::PointSet* pointSet = 0;
  if(selectedNode)
    pointSet = dynamic_cast<mitk::PointSet*> ( selectedNode->GetData() );

  if (pointSet)
  {
    m_SelectedPointSetNode = selectedNode;
    m_Controls->m_CurrentPointSetLabel->setText(QString::fromStdString(selectedNode->GetName()));
    m_Controls->m_PointListWidget->SetPointSetNode(selectedNode);
  }
  else
  {
    m_Controls->m_CurrentPointSetLabel->setText("None");
    m_Controls->m_PointListWidget->SetPointSetNode(0);
  }

}

bool QmitkPathTrackView::IsExclusiveFunctionality() const
{
  return true;
}

void QmitkPathTrackView::NodeChanged( const mitk::DataNode* node )
{
  if(node == m_SelectedPointSetNode && m_Controls->m_CurrentPointSetLabel->text().toStdString() != node->GetName())
  {
    m_Controls->m_CurrentPointSetLabel->setText(QString::fromStdString(node->GetName()));
  }
}

void QmitkPathTrackView::StdMultiWidgetAvailable( QmitkStdMultiWidget& stdMultiWidget )
{
    if(m_Controls)
      m_Controls->m_PointListWidget->SetMultiWidget( &stdMultiWidget );
}

void QmitkPathTrackView::StdMultiWidgetClosed( QmitkStdMultiWidget& /*stdMultiWidget*/ )
{
   if(m_Controls)
      m_Controls->m_PointListWidget->SetMultiWidget( 0 );
}
