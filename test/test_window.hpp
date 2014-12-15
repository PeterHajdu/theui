#pragma once

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
    }

    bool was_event_dispatched{ false };
    void handle_event( const AnEvent& )
    {
      was_event_dispatched = true;
    }
};

}

