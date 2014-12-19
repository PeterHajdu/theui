#pragma once

#include <theui/pointer.hpp>

namespace test
{

class AnEvent
{
  public:
    add_ctci( "test_an_event" );
};

class Window : public the::ui::Window
{
  public:
    Window()
    {
      m_dispatcher.register_listener< AnEvent >(
          [ this ]( const AnEvent& event )
          {
            handle_event( event );
          } );

      m_dispatcher.register_listener< the::ui::Arrive >(
          [ this ]( const the::ui::Arrive& event )
          {
            handle_arrive( event );
          } );

      m_dispatcher.register_listener< the::ui::Leave >(
          [ this ]( const the::ui::Leave& event )
          {
            handle_leave( event );
          } );

      m_dispatcher.register_listener< the::ui::Resized >(
          [ this ]( const the::ui::Resized& event )
          {
            handle_resized( event );
          } );
    }

    bool was_event_dispatched{ false };
    void handle_event( const AnEvent& )
    {
      was_event_dispatched = true;
    }

    bool did_arrive{ false };
    void handle_arrive( const the::ui::Arrive& )
    {
      did_arrive = true;
    }

    bool did_leave{ false };
    void handle_leave( const the::ui::Leave& )
    {
      did_leave = true;
    }

    bool was_resized{ false };
    void handle_resized( const the::ui::Resized& )
    {
      was_resized = true;
    }
};

}

