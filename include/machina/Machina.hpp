#ifndef BEBOP_MACHINA_MACHINA_HPP
#define BEBOP_MACHINA_MACHINA_HPP

#include <stdexcept>
#include <tuple>
#include <type_traits>

namespace bebop::machina
{

template <typename Derived, typename State>
struct Machina
{
  static_assert(std::is_enum_v<State>, "State type must be an enumeration");

  template <State s1, State s2, typename... Args>
  struct Transition
  {
    static constexpr auto From = s1;
    static constexpr auto To = s2;

    using ArgTypes = std::tuple<Args...>;
  };

  constexpr Machina(State initial) : m_state(initial), m_initial(initial) {}

  template <typename Transition, typename... Args>
  void next(Args&&... args)
  {
    static_assert(std::is_same_v<typename Transition::ArgTypes, std::tuple<std::decay_t<Args>...>>,
                  "Argument types do not match transition!");

    if (m_state != Transition::From)
      throw std::runtime_error("Invalid transition: Current state is different");

    m_state = Transition::To;

    // The owner of the args is the new state now
    static_cast<Derived*>(this)->onEnter(Transition{}, std::move(args)...);
  }

  void reset()
  {
    m_state = m_initial;
    static_cast<Derived*>(this)->onReset();
  }

 protected:
  State m_state;
  State m_initial;
};
}  // namespace bebop::machina

#endif  // BEBOP_MACHINA_MACHINA_HPP