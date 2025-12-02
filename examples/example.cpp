#include "fsm/fsm.hpp"

#include <iostream>
#include <string>

enum class State {
  State0,
  State1,
  State2
};

enum class Event {
  Event1,
  Event2
};

static std::string StateToString(const State &state) {
  switch (state) {
  case State::State0:
    return "State0";
  case State::State1:
    return "State1";
  case State::State2:
    return "State2";
  }
  return "Unknown";
}

namespace Action {
const auto Action1{[]() { std::cout << "Action1" << std::endl; }};
const auto Action2{[]() { std::cout << "Action2" << std::endl; }};
} // namespace Action

namespace Guard {
const auto Guard1{[]() { return true; }};
const auto Guard2{[]() { return true; }};
const auto Guard3{[]() { return false; }};
} // namespace Guard

int main() {
  const fsm::TransitionTable<State, Event> transitionTable{
      {{State::State0, Event::Event1}, {Guard::Guard1, Action::Action1, State::State1}},
      {{State::State1, Event::Event2}, {Guard::Guard2, Action::Action2, State::State2}},
      {{State::State2, Event::Event1}, {Guard::Guard3, Action::Action1, State::State1}},
  };

  fsm::FiniteStateMachine<State, Event> sm(State::State0, transitionTable);
  std::cout << StateToString(sm.GetState()) << std::endl;

  sm.SetEnterAction(State::State1, []() { std::cout << "EnterAction1" << std::endl; });
  sm.SetLeaveAction(State::State1, []() { std::cout << "LeaveAction1" << std::endl; });

  sm.HandleEvent(Event::Event1);
  std::cout << StateToString(sm.GetState()) << std::endl;

  sm.HandleEvent(Event::Event2);
  std::cout << StateToString(sm.GetState()) << std::endl;

  sm.HandleEvent(Event::Event1);
  std::cout << StateToString(sm.GetState()) << std::endl;

  return 0;
}
