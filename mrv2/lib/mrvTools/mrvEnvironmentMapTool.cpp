// SPDX-License-Identifier: BSD-3-Clause
// mrv2 (mrViewer2)
// Copyright Contributors to the mrv2 Project. All rights reserved.

#include <FL/Fl_Flex.H>

#include "mrvWidgets/mrvHorSlider.h"
#include "mrvWidgets/mrvFunctional.h"

#include "mrvTools/mrvToolsCallbacks.h"

#include "mrViewer.h"


namespace mrv
{

    EnvironmentMapTool::EnvironmentMapTool( ViewerUI* ui ) :
        ToolWidget( ui )
    {

        add_group( _("Environment Map") );

        Fl_SVG_Image* svg = load_svg( "EnvironmentMap.svg" );
        g->image( svg );


        ui->uiView->setActionMode( ActionMode::kRotate );
        
        g->callback( []( Fl_Widget* w, void* d ) {
            ViewerUI* ui = static_cast< ViewerUI* >( d );
            delete environmentMapTool; environmentMapTool = nullptr;
            ui->uiView->setActionMode( ActionMode::kScrub );
            ui->uiView->redrawWindows();
            ui->uiMain->fill_menu( ui->uiMenuBar );
        }, ui );

    }

    EnvironmentMapTool::~EnvironmentMapTool()
    {
        clear_controls();
    }

    void EnvironmentMapTool::clear_controls()
    {
        delete hAperture;
        delete vAperture;
        delete focalLength;
        delete rotateX;
        delete rotateY;
    }

    void EnvironmentMapTool::add_controls()
    {
        TLRENDER_P();

        g->clear();
        g->begin();

        HorSlider* s;
        CollapsibleGroup* cg = new CollapsibleGroup( g->x(), 20, g->w(), 20,
                                                     _("Type") );
        auto b = cg->button();
        b->labelsize(14);
        b->size(b->w(), 18);
        cg->layout();
        cg->begin();

        Fl_Flex* flex = new Fl_Flex( g->x(), 20, g->w(), 20 );
        flex->type( Fl_Flex::HORIZONTAL );

        flex->begin();
        
        sphericalMap = new Fl_Radio_Round_Button( g->x(), 20, g->w(), 20,
                                                _("Spherical") );
        sphericalMap->value(1);
        
        cubicMap = new Fl_Radio_Round_Button( g->x(), 20, g->w(), 20,
                                              _("Cubic") );

        flex->end();

        cg->end();
        
        cg = new CollapsibleGroup( g->x(), 20, g->w(), 20, _("Projection") );
        b = cg->button();
        b->labelsize(14);
        b->size(b->w(), 18);
        cg->layout();
        cg->begin();
        
        auto sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20,
                                           _("H. Aperture") );
        s = hAperture = sV;
        s->tooltip(
            _( "Horizontal Aperture of the Projection.")
        );
        s->range( 0.f, 90.0f );
        s->step( 0.01F );
        s->default_value( 24.0f );
        sV->callback( [=]( auto w ) {
            p.ui->uiView->redrawWindows();
        } );


        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20,
                                      _("V. Aperture") );
        s = vAperture = sV;
        s->tooltip(
            _( "Vertical Aperture of the Projection.")
        );
        s->range( 0.f, 90.0f );
        s->step( 0.1F );
        s->default_value( 0.f );
        sV->callback( [=]( auto w ) {
            p.ui->uiView->redrawWindows();
        } );

        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20,
                                      _("Focal Length") );
        s = focalLength = sV;
        s->tooltip(
            _( "Focal Length of the Projection.")
        );
        s->range( 0.1f, 50.0f );
        s->step( 0.1F );
        s->default_value( 7.f );
        sV->callback( [=]( auto w ) {
            p.ui->uiView->redrawWindows();
        } );

        cg->end();

        cg = new CollapsibleGroup( g->x(), 20, g->w(), 20, _("Rotation") );
        b = cg->button();
        b->labelsize(14);
        b->size(b->w(), 18);
        cg->layout();
        cg->begin();

        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "X" );
        s = rotateX = sV;
        s->tooltip( _("Rotation in X of the sphere." ) );
        s->range( 45.f, 135.0f );
        s->default_value( 90.0f );
        sV->callback( [=]( auto w ) {
            p.ui->uiView->redrawWindows();
        } );


        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "Y" );
        s = rotateY = sV;
        s->tooltip( _("Rotation in Y of the sphere." ) );
        s->range( 0.f, 360.0f );
        s->default_value( 180.0f );
        sV->callback( [=]( auto w ) {
            p.ui->uiView->redrawWindows();
        } );


        cg->end();


        g->end();
        
        p.ui->uiView->redrawWindows();

    }


}