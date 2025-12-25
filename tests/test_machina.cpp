#include <gtest/gtest.h>

#include <iostream>
#include <machina/Machina.hpp>
#include <vector>

enum class State
{
  Initial,
  Working,
  Finished
};

struct StateMachine : bebop::machina::Machina<StateMachine, State>
{
  using InitialToWorking = Transition<State::Initial, State::Working>;
  using WorkingToFinished = Transition<State::Working, State::Finished, std::string, std::string>;

  constexpr StateMachine(State state) : bebop::machina::Machina<StateMachine, State>(state) {}

  template <typename Transition, typename... Args>
  void onEnter(Transition, Args&&...)
  {
  }

  const std::vector<std::string>& data() const { return m_data; }

  State currentState() const { return m_state; }

  void onEnter(InitialToWorking)
  {
    // Do nothing
  }

  void onEnter(WorkingToFinished, std::string&& arg1, std::string&& arg2)
  {
    m_data.emplace_back(std::move(arg1));
    m_data.emplace_back(std::move(arg2));
  }

  void onReset() { m_data.clear(); }

 private:
  std::vector<std::string> m_data;
};

TEST(StateMachine, StandardWorkflow)
{
  StateMachine stateMachine(State::Initial);
  EXPECT_EQ(stateMachine.currentState(), State::Initial);

  stateMachine.next<StateMachine::InitialToWorking>();
  EXPECT_EQ(stateMachine.currentState(), State::Working);

  std::string hello = "hello, ";
  std::string world = "world, ";

  stateMachine.next<StateMachine::WorkingToFinished>(std::move(hello), std::move(world));
  EXPECT_EQ(stateMachine.currentState(), State::Finished);

  EXPECT_EQ(stateMachine.data().size(), 2);
  EXPECT_EQ(stateMachine.data()[0], "hello, ");
  EXPECT_EQ(stateMachine.data()[1], "world, ");

  stateMachine.reset();
  EXPECT_EQ(stateMachine.currentState(), State::Initial);
}

TEST(StateMachine, WrongTransition)
{
  StateMachine stateMachine(State::Initial);

  std::string hello;
  std::string world;

  EXPECT_THROW(stateMachine.next<StateMachine::WorkingToFinished>(std::move(hello), std::move(world)), std::runtime_error);

  EXPECT_NO_THROW(stateMachine.reset());
}
