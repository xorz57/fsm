// MIT License
//
// Copyright (c) 2025 Georgios Fotopoulos
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <algorithm>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace fsm {
using Guard = std::function<bool()>;

using Action = std::function<void()>;

using EnterAction = std::function<void()>;

using LeaveAction = std::function<void()>;

template <typename State>
using EnterActions = std::unordered_map<State, EnterAction>;

template <typename State>
using LeaveActions = std::unordered_map<State, LeaveAction>;

template <typename State, typename Event>
using Transition = std::pair<std::pair<State, Event>, std::tuple<Guard, Action, State>>;

template <typename State, typename Event>
using TransitionTable = std::vector<Transition<State, Event>>;

template <typename State, typename Event>
class FiniteStateMachine {
public:
  FiniteStateMachine() = default;

  FiniteStateMachine(State state, TransitionTable<State, Event> transitionTable) : m_State(std::move(state)), m_TransitionTable(std::move(transitionTable)) {}

  bool HandleEvent(Event const &event) {
    auto const &transitionTableIterator{std::find_if(m_TransitionTable.begin(), m_TransitionTable.end(), [&](Transition<State, Event> const &transition) {
      return transition.first.first == m_State && transition.first.second == event;
    })};
    if (transitionTableIterator != m_TransitionTable.end()) {
      auto &transition = transitionTableIterator->second;
      Guard &guard = std::get<0>(transition);
      Action &action = std::get<1>(transition);
      State &state = std::get<2>(transition);
      if (std::invoke(guard)) {
        auto const &leaveActionsIterator{m_LeaveActions.find(m_State)};
        if (leaveActionsIterator != m_LeaveActions.end()) {
          std::invoke(leaveActionsIterator->second);
        }
        std::invoke(action);
        m_State = state;
        auto const &enterActionsIterator{m_EnterActions.find(m_State)};
        if (enterActionsIterator != m_EnterActions.end()) {
          std::invoke(enterActionsIterator->second);
        }
        return true;
      }
      return true;
    }
    return false;
  }

  void SetState(State state) {
    m_State = std::move(state);
  }

  void SetTransitionTable(TransitionTable<State, Event> transitionTable) {
    m_TransitionTable = std::move(transitionTable);
  }

  void SetEnterAction(State const &state, EnterAction enterAction) {
    m_EnterActions[state] = std::move(enterAction);
  }

  void SetLeaveAction(State const &state, LeaveAction leaveAction) {
    m_LeaveActions[state] = std::move(leaveAction);
  }

  State GetState() const {
    return m_State;
  }

  const TransitionTable<State, Event> &GetTransitionTable() const {
    return m_TransitionTable;
  }

  const EnterActions<State> &GetEnterActions() const {
    return m_EnterActions;
  }

  const LeaveActions<State> &GetLeaveActions() const {
    return m_LeaveActions;
  }

private:
  State m_State;
  TransitionTable<State, Event> m_TransitionTable;
  EnterActions<State> m_EnterActions;
  LeaveActions<State> m_LeaveActions;
};
} // namespace fsm
