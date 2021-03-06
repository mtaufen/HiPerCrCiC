// Michael Taufen 2012
#include "EventDispatcher.h"
#include <iostream>

EventDispatcher::EventDispatcher()
{
  _currentFocus = 0; // should always be null unless something has focus
}

void EventDispatcher::registerMouseListener(View * view)
{
  _mouseEventListeners.insert(view);
}

void EventDispatcher::registerKeyboardListener(View * view)
{
  _keyboardEventListeners.insert(view);
}

void EventDispatcher::registerCrawlerListener(View * view)
{
  _crawlerEventListeners.insert(view);
}

void EventDispatcher::registerJobManagerListener(View * view)
{
  _jobManagerEventListeners.insert(view);
}

void EventDispatcher::registerQueryEngineListener(View * view)
{
  _queryEngineEventListeners.insert(view);
}

bool EventDispatcher::removeMouseListenerWithPointer(View * const viewPointer)
{
  set<View*>::iterator it = _mouseEventListeners.find(viewPointer);
  if (it != _mouseEventListeners.end())
  {
    _mouseEventListeners.erase(it);
    return true;
  }
  else
  {
    std::cout << "Error:\tNo listener could be matched with provided pointer. [In: EventDispatcher::removeKeyboardListenerWithPointer(View * const viewPointer)]" << std::endl;
    return false;
  }
}

bool EventDispatcher::removeKeyboardListenerWithPointer(View * const viewPointer)
{
  set<View*>::iterator it = _keyboardEventListeners.find(viewPointer);
  if (it != _keyboardEventListeners.end())
  {
    _keyboardEventListeners.erase(it);
    return true;
  }
  else
  {
    std::cout << "Error:\tNo listener could be matched with provided pointer. [In: EventDispatcher::removeKeyboardListenerWithPointer(View * const viewPointer)]" << std::endl;
    return false;
  }
}

bool EventDispatcher::removeCrawlerListenerWithPointer(View * const viewPointer)
{
  set<View*>::iterator it = _crawlerEventListeners.find(viewPointer);
  if (it != _crawlerEventListeners.end())
  {
    _crawlerEventListeners.erase(it);
    return true;
  }
  else
  {
    std::cout << "Error:\tNo listener could be matched with provided pointer. [In: EventDispatcher::removeCrawlerListenerWithPointer(View * const viewPointer)]" << std::endl;
    return false;
  }
}

bool EventDispatcher::removeJobManagerListenerWithPointer(View * const viewPointer)
{
  set<View*>::iterator it = _jobManagerEventListeners.find(viewPointer);
  if (it != _jobManagerEventListeners.end())
  {
    _jobManagerEventListeners.erase(it);
    return true;
  }
  else
  {
    std::cout << "Error:\tNo listener could be matched with provided pointer. [In: EventDispatcher::removeJobManagerListenerWithPointer(View * const viewPointer)]" << std::endl;
    return false;
  }
}

bool EventDispatcher::removeQueryEngineListenerWithPointer(View * const viewPointer)
{
  set<View*>::iterator it = _queryEngineEventListeners.find(viewPointer);
  if (it != _queryEngineEventListeners.end())
  {
    _queryEngineEventListeners.erase(it);
    return true;
  }
  else
  {
    std::cout << "Error:\tNo listener could be matched with provided pointer. [In: EventDispatcher::removeQueryEngineListenerWithPointer(View * const viewPointer)]" << std::endl;
    return false;
  }
}

void EventDispatcher::pushMouseEvent(MouseEvent e)
{
  _mouseEventQueue.push(e);
}

void EventDispatcher::pushKeyboardEvent(KeyboardEvent e)
{
  _keyboardEventQueue.push(e);
}

void EventDispatcher::pushCrawlerEvent(CrawlerEvent e)
{
  _crawlerEventQueue.push(e);
}

void EventDispatcher::pushJobManagerEvent(JobManagerEvent e)
{
  _jobManagerEventQueue.push(e);
}

void EventDispatcher::pushQueryEngineEvent(QueryEngineEvent e)
{
  _queryEngineEventQueue.push(e);
}

void EventDispatcher::processNextMouseEvent()
{
  if (_mouseEventQueue.size() > 0)
  {
    MouseEvent e(_mouseEventQueue.front());
    if (_mouseEventListeners.size() > 0)
    {
      bool handled = false;
      for (set<View*>::iterator it = _mouseEventListeners.begin();
       it != _mouseEventListeners.end() && !handled; ++it)
      {
        if ( (*it)->getCanRecieve() )// if the listener is currently able to recieve events
        {
          switch(e.getType())
          {
            case MOUSE_CLICK:
              if (e.getButton() == LEFT_MOUSE_BUTTON)
                handled = (*it)->onLeftClick(e.getPosition());
              else if (e.getButton() == RIGHT_MOUSE_BUTTON)
                handled = (*it)->onRightClick(e.getPosition());
  
              if ( handled && (*it)->getFocusable() ) // give focus if focusable TODO: (sort of cheating here)
              {
                _currentFocus = (*it);
                _currentFocus->onFocusIn();
              }
              else // if this is a non-focusable element, remove focus from whatever has it
              {
                if (_currentFocus)
                  _currentFocus->onFocusOut();
                _currentFocus = 0;
              }
              break;
            case MOUSE_UP:
              handled = (*it)->onMouseUp(e.getPosition());
              break;
            case MOUSE_DOWN:
              handled = (*it)->onMouseDown(e.getPosition());
              break;
            case MOUSE_MOVE:
              handled = (*it)->onMouseMove(e.getPosition());
              break;
            case MOUSE_OVER:
              handled = (*it)->onMouseOver(e.getPosition());
              break;
            case MOUSE_OUT:
              handled = (*it)->onMouseOut(e.getPosition());
              break;
            case MOUSE_DRAG:
              handled = (*it)->onMouseDrag(e.getPosition());
            default:
              // do nothing, event is discarded
              break;
          }
        }
      }
    }
    _mouseEventQueue.pop();
  }
}

void EventDispatcher::processNextKeyboardEvent()
{
  if (_keyboardEventQueue.size() > 0)
  {
    KeyboardEvent e(_keyboardEventQueue.front());
    if (_keyboardEventListeners.size() > 0)
    {
      if (!_currentFocus)
      {
        bool handled = false;
        for (set<View*>::iterator it = _keyboardEventListeners.begin();
         it != _keyboardEventListeners.end() && !handled; ++it)
        {
          switch(e.getType())
          {
            case KEY_PRESS:
              handled = (*it)->onKeyDown(e.getKey());
              break;
            case KEY_DOWN:
              handled = (*it)->onRightClick(e.getKey());
              break;
            case KEY_UP:
              handled = (*it)->onMouseUp(e.getKey());
              break;
            default:
              // do nothing, event is discarded
              break;
          }
        }
      }
      // pass the keyboard events to whatever view has focus, if it is an event listener
      else if (_keyboardEventListeners.find(_currentFocus) != _keyboardEventListeners.end() )
      {
        switch(e.getType())
        {
          case KEY_PRESS:
            (_currentFocus)->onKeyDown(e.getKey());
            break;
          case KEY_DOWN:
            (_currentFocus)->onRightClick(e.getKey());
            break;
          case KEY_UP:
            (_currentFocus)->onMouseUp(e.getKey());
            break;
          default:
            // do nothing, event is discarded
            break;
        }
      }
    }
    _keyboardEventQueue.pop();
  }
}

void EventDispatcher::processNextCrawlerEvent()
{
  if (_crawlerEventQueue.size() > 0)
  {
    CrawlerEvent e(_crawlerEventQueue.front());
    if (_crawlerEventListeners.size() > 0)
    {      
      for (set<View*>::iterator it = _crawlerEventListeners.begin();
       it != _crawlerEventListeners.end(); ++it)
      {
        switch(e.getType())
        {
          case CRAWLER_UPDATE:
            (*it)->onCrawlerUpdate();
            break;
          default:
            // do nothing, event is discarded
            break;
        }
      }
    }
    _crawlerEventQueue.pop();
  }
}

void EventDispatcher::processNextJobManagerEvent()
{
  if (_jobManagerEventQueue.size() > 0)
  {
    JobManagerEvent e(_jobManagerEventQueue.front());
    if (_jobManagerEventListeners.size() > 0)
    {      
      for (set<View*>::iterator it = _jobManagerEventListeners.begin();
       it != _jobManagerEventListeners.end(); ++it)
      {
        switch(e.getType())
        {
          case JOB_ADDED:
            (*it)->onJobAdded(e.getAssociatedJob());
            break;
          default:
            // do nothing, event is discarded
            break;
        }
      }
    }
    _jobManagerEventQueue.pop();
  }
}

void EventDispatcher::processNextQueryEngineEvent()
{
  if (_queryEngineEventQueue.size() > 0)
  {
    QueryEngineEvent e(_queryEngineEventQueue.front());
    if (_queryEngineEventListeners.size() > 0)
    {      
      for (set<View*>::iterator it = _queryEngineEventListeners.begin();
       it != _queryEngineEventListeners.end(); ++it)
      {
        switch(e.getType())
        {
          case RESULT_FOUND:
            (*it)->onResultFound(e.getResultInfo());
            break;
          default:
            // do nothing, event is discarded
            break;
        }
      }
    }
    _queryEngineEventQueue.pop();
  }
}


void EventDispatcher::eventLoop()
{
  processNextMouseEvent();
  processNextKeyboardEvent();
  processNextCrawlerEvent();
  processNextJobManagerEvent();
  processNextQueryEngineEvent();
}