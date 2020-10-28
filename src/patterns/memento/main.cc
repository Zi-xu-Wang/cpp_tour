#include <iostream>
#include <list>
#include <string>
#include <vector>

class Memento {
 private:
  std::string state;

 public:
  Memento(std::string stateToSave) { state = stateToSave; }

  std::string getSavedState() { return state; }
};
//-----------------------------------------------------
class Originator {
 private:
  std::string state;
  // The class could also contain additional data that is not part of the
  // state saved in the memento..

 public:
  void set(std::string state) {
    this->state = state;
    std::cout << "Originator: Setting state to " + state << std::endl;
  }

  Memento saveToMemento() {
    std::cout << "Originator: Saving to Memento." << std::endl;
    return Memento(this->state);
  }

  void restoreFromMemento(Memento memento) {
    this->state = memento.getSavedState();
    std::cout << "Originator: State after restoring from Memento: " + state
              << std::endl;
  }
};
//-----------------------------------------------------
// Caretaker
int main() {
  std::vector<Memento> savedStates;

  Originator originator;
  originator.set("State1");
  originator.set("State2");
  savedStates.push_back(originator.saveToMemento());
  originator.set("State3");
  // We can request multiple mementos, and choose which one to roll back to.
  savedStates.push_back(originator.saveToMemento());
  originator.set("State4");

  originator.restoreFromMemento(savedStates[1]);

  return 0;
}
