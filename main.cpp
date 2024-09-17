#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

/*
Kirjoita tätä käyttötapausta varten sovellus, johon pystyy kirjaamaan ylös
tehtäviä asioita tälle päivälle.
- Aina kun jokin tehtävä on suoritettu, voi sen merkata suoritetuksi.
- Tehtäviä pitää myös pystyä poistamaan listalta.
Uudelleenkäynnistyksen yhteydessä sovelluksen pitää pystyä palauttamaan se tila,
mikä oli sovellusta suljettaessa.
*/

/* 
tiedostomuoto: <char: suoritettu> <char array: tehtävä>
esimerkki tiedoston sisällöstä:
1 aloita tehtävä
1 lopeta tehtävä
1 palauta tehtävä
0 saa palaute tehtävästä
*/
const auto FILE_NAME = "tasks.txt";

namespace
{
size_t select()
{
    size_t choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void clear() {
#ifdef _WIN32
    system("cls"); // untested
#else
    system("clear");
#endif
}
}

namespace task
{
// Rakenne johon rivi luetaan
struct Task {
    std::string description;
    bool completed;

    Task(std::string desc, bool comp = false) : description(desc), completed(comp) {}
};

// Lataa tehtävät
std::vector<Task> load() {
    std::vector<Task> tasks;
    std::ifstream file(FILE_NAME);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            bool completed = (line[0] == '1');
            std::string description = line.substr(2);
            tasks.emplace_back(description, completed);
        }
        file.close();
    }
    return tasks;
}

// Tallenna tehtävät
void save(const std::vector<Task>& tasks) {
    std::ofstream file(FILE_NAME);
    for (const auto& task : tasks) {
        file << (task.completed ? '1' : '0') << " " << task.description << std::endl;
    }
    file.close();
}

// Näytä tehtävät
void display(const std::vector<Task>& tasks) {
    if (tasks.empty()) return;
    std::cout << "\nTehtävälista:" << std::endl;
    for (size_t i = 0; i < tasks.size(); ++i) {
        std::cout << i + 1 << ". " << (tasks[i].completed ? "[x] " : "[ ] ") << tasks[i].description << std::endl;
    }
}

// 1. Lisää
void add(std::vector<Task>& tasks) {
    std::cout << "Uusi tehtävä: ";
    std::string description;
    std::getline(std::cin, description);
    tasks.emplace_back(description);
    save(tasks);
}

// 2. Merkitse suoritetuksi
void complete(std::vector<Task>& tasks) {
    std::cout << "Tehtävän numero: ";
    const auto index = select();
    if (index > 0 && index <= tasks.size()) {
        tasks[index - 1].completed = true;
        save(tasks);
    } else {
        std::cout << "Tehtävää ei löytynyt." << std::endl;
    }
}

// 3. Poista
void remove(std::vector<Task>& tasks) {
    std::cout << "Tehtävän numero: ";
    const auto index = select();
    if (index > 0 && index <= tasks.size()) {
        tasks.erase(tasks.begin() + index - 1);
        save(tasks);
    } else {
        std::cout << "Tehtävää ei löytynyt." << std::endl;
    }
}

// Päävalikko
void menu() {
    std::cout << "\nKomennnot:\n";
    std::cout << "1. Lisää\n";
    std::cout << "2. Merkitse suoritetuksi\n";
    std::cout << "3. Poista\n";
    std::cout << "4: Poistu\n";
    std::cout << "Valinta: ";
}
}

int main() {
    auto tasks = task::load();

    while(true) {
        clear();
        task::display(tasks);
        task::menu();
        const auto choice = select(); 

        switch (choice) {
            case 1:
                task::add(tasks);
                break;
            case 2:
                task::complete(tasks);
                break;
            case 3:
                task::remove(tasks);
                break;
            default:
                std::cout << "Virheellinen valinta" << std::endl;
                [[fallthrough]];
            case 4:
                return 0;
        }
    };

    return 0;
}
