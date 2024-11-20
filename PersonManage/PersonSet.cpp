#include "PersonSet.h"

PersonSet::PersonSet() {}

PersonSet::~PersonSet() {}

bool PersonSet::isDuplicate(const Person& person) {
    return find(persons.begin(), persons.end(), person) != persons.end();
}

void PersonSet::push_back(const Person& person) {
    if (!isDuplicate(person)) {
        persons.push_back(person);
    }
}

void PersonSet::push_front(const Person& person) {
    if (!isDuplicate(person)) {
        persons.push_front(person);
    }
}

void PersonSet::pop_back() {
	persons.pop_back();
}

Person PersonSet::removeAt(int index) {
    Person person = persons.at(index);
	persons.erase(persons.begin() + index);
	return person;
}

void PersonSet::insert(int index, const Person& person) {
	if (!isDuplicate(person)) {
		persons.insert(persons.begin() + index, person);
	}
}

void PersonSet::insert(deque<Person>::iterator pos, const Person& person) {
    if (!isDuplicate(person)) {
        persons.insert(pos, person);
    }
}

void PersonSet::insert(deque<Person>::iterator pos, int n, const Person& person) {
    if (!isDuplicate(person)) {
        persons.insert(pos, n, person);
    }
}

void PersonSet::insert(deque<Person>::iterator pos, deque<Person>::iterator first, deque<Person>::iterator last) {
    for (auto it = first; it != last; ++it) {
        if (!isDuplicate(*it)) {
            persons.insert(pos, *it);
            pos++;
        }
    }
}

void PersonSet::deletePersonById(string id) {
    for(auto it = persons.begin(); it != persons.end(); ++it) {
		if (it->GetId() == id) {
			persons.erase(it);
			break;
		}
	}
}

void PersonSet::editPerson(Person editPerson, const Person& person) {
	for(auto& p : persons) {
		if(p.GetId() == editPerson.GetId()) {
            p = person;
            break;
		}
	}
}

Person PersonSet::getPersonById(string id) {
	for (const auto& person : persons) {
		if (person.GetId() == id) {
			return person;
		}
	}
	return Person();
}

int PersonSet::indexOf(const Person& person) {
	auto it = find(persons.begin(), persons.end(), person);
	if (it != persons.end()) {
		return distance(persons.begin(), it);
	}
	return -1;
}

void PersonSet::saveToFile(const string& filename) {
    json j_array = json::array();
    for (const auto& person : persons) {
        j_array.push_back(person.toJson());
    }
    ofstream file(filename);
    if (file.is_open()) {
        file << j_array.dump(4); // 格式化输出
        file.close();
    }
    else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }
}

void PersonSet::loadFromFile(const string& filename) {
    json j_array_from_file;
    ifstream inputFile(filename);
    if (inputFile.is_open()) {
        inputFile >> j_array_from_file;
        inputFile.close();
        persons.clear(); // 清除当前内容
        for (const auto& j : j_array_from_file) {
            persons.push_back(Person::from_json(j));
        }
    }
    else {
        std::cerr << "Unable to open file for reading" << std::endl;
    }
}

void PersonSet::printAll() const {
    for (const auto& person : persons) {
        std::cout << person << std::endl;
    }
}

deque<Person>& PersonSet::getAllPersons() {
    return persons;
}