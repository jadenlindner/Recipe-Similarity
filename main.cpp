// SFML Library
#include <SFML/Graphics.hpp>
// Classes
#include "searchBar.h"
#include "MaxHeap.h"
#include "RedBlackTree.h"
// C++ Libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <iterator>
#include <unordered_map>
#include "ResourcePath.hpp"
using namespace std;

// ensure only letters & spaces in ingredients stay for proper comparisons later
string onlyAlphabet(string str) {
    for (int i = 0; i < str.size(); i++) {
        if (str.at(i) != ' ' && (str.at(i) < 'A' || str.at(i) > 'Z') && (str.at(i) < 'a' || str.at(i) > 'z')) {
            str.erase(i, 1);
            i--;
        }
    }
    return str;
}

// extract the ingredients list from recipe
set<string> createIngredientList(string ingredients, int numIngredients) {
    set<string> ingredientSet;

    for (int i = 0; i < numIngredients - 1; i++) {
        int index = ingredients.find(',');
        string temp = ingredients.substr(0, index); // ingredient name

        ingredients = ingredients.substr(index + 2, ingredients.size() - temp.size() - 2);
        temp = onlyAlphabet(temp);
        ingredientSet.insert(temp);
    }
    ingredients = onlyAlphabet(ingredients);
    ingredientSet.insert(ingredients);

    return ingredientSet;
}

// read file function
unordered_map<string, set<string>> ReadFile(string path) {
    char delimiter = '|';
    string name;
    string ingredientTemp;
    set<string> ingredients;
    int numIngredients;
    ifstream file(path);
    string row;
    unordered_map<string, set<string>> recipes; // all recipe names & ingredients

    getline(file, row);
    while (getline(file, row)) { // while not empty
        stringstream stream(row);
        string value;
        string temp;

        while (getline(stream, value, delimiter)) {
            name = value;
            getline(stream, value, delimiter);  // id
            getline(stream, value, delimiter);  // minutes
            getline(stream, value, delimiter);  // contributors
            getline(stream, value, delimiter);  // date
            getline(stream, value, delimiter);  // tags
            getline(stream, value, delimiter);  // nutrition
            getline(stream, value, delimiter);  // # steps
            getline(stream, value, delimiter);  // steps
            getline(stream, value, delimiter);  // description
            getline(stream, value, delimiter);  // ingredients
            ingredientTemp = value;
            getline(stream, value, delimiter);  // # ingredients
            numIngredients = stoi(value);

            ingredients = createIngredientList(ingredientTemp, numIngredients);
            recipes[name] = ingredients;
        }

    }

    return recipes;
}

// convert to all uppercase in order to compare properly
string convertToUpper(string str) {
  for (int i = 0; i < str.length(); i++) {
    // removes spaces
    if (str[i] == ' ') {
      str.erase(i, 1);
    }
    // converts to uppercase
    if (str[i] != ' ') {
      str[i] = toupper(str[i]);
    }
  }
  return str;
}

// searching data for matching recipes to the user input
vector<string> searchFunction(string userSearch, unordered_map<string, set<string>> recipesDatabase) {
    vector<string> recipeMatches;
    for (auto it = recipesDatabase.begin(); it != recipesDatabase.end(); ++it) {
        if (convertToUpper(it->first).find(convertToUpper(userSearch)) != string::npos) {   recipeMatches.push_back(it->first);
        }
    }
    return recipeMatches;
}

int main(int, char const**) {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "Recipe Similarity Program");

    // ------------------------------- Create the GUI to display  ----------------------------- //
    sf::Font font;
    
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    
    sf::Text text("Recipe Similarity", font, 50);
    text.setFillColor(sf::Color::White);
    // center the title font/text
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(1500/2.0f,200/2.0f));
    
    // description of what to do
    sf::Text descript("Type in a recipe you want to find similar recipes to. Recipes sourced from Food.com", font, 30);
    descript.setFillColor(sf::Color::White);
    // position description
    descript.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    descript.setPosition(sf::Vector2f(800/2.0f,300/2.0f));
    
    // log where the mouse is on window
    sf::Vector2i winpos = sf::Mouse::getPosition(window);
    
    // search bar/ input text field
    string recipeInput = "";
    sf::RectangleShape inputBox(sf::Vector2f(1000, 50));
    inputBox.setPosition(275,150);
    
    // show the user what they are typing in search bar
    searchBar text1(100, sf::Color::White, true);
    text1.setPosition({ 200, 0 });
    text1.setFont(font);
    
    sf::Text chooseRecipe("Choose a recipe in the drop down and find similar recipes", font, 30);
    chooseRecipe.setFillColor(sf::Color::White);
    chooseRecipe.setOrigin(40,-60);
    chooseRecipe.setPosition(sf::Vector2f(800/2.0f,300/2.0f));
    
    // drop down menu that includes the input similarities for the user to choose from
    vector<string> dropDownOptions;
    sf::RectangleShape dropDownBox(sf::Vector2f(700, 50));
    dropDownBox.setPosition(380, 250);
    bool dropDownEnable = false;
    bool displayOptions = false;
    
    // drop down options list
    sf::RectangleShape options;
    options.setPosition(380, 301);
    sf::Text optionChoice("", font, 30);
    optionChoice.setFillColor(sf::Color::Black);
    
    // refresh button
    sf::RectangleShape refreshButton(sf::Vector2f(150, 50));
    refreshButton.setPosition(1100, 250);
    sf::Text refreshTitle("Refresh", font, 30);
    refreshTitle.setPosition(1120, 255);
    refreshTitle.setFillColor(sf::Color::Black);
    bool refresh = false;
    int counter = 0;
    
    int division = 0;
    int remainder = 0;
    int dimensions = 0;
    
    // display the chosen drop down recipe by user
    string recipeChosen = "";
    sf::Text dropDownRecipe(recipeChosen, font, 30);
    dropDownRecipe.setPosition(380, 250);
    dropDownRecipe.setFillColor(sf::Color::Black);
    
    // two boxes to hold the outputs of each of the data structures
    // size of each box is 710 pixels
    sf::Text dataTitle1("Red Black Tree", font, 30);
    dataTitle1.setPosition(20, 350);
    sf::Text dataTitle2("Max Heap", font, 30);
    dataTitle2.setPosition(770, 350);
    sf::RectangleShape data1(sf::Vector2f(710, 500));
    data1.setPosition(20, 400);
    sf::RectangleShape data2(sf::Vector2f(710, 500));
    data2.setPosition(770, 400);
    
    // output matching recipes
    sf::Text outputs1("", font, 30);
    outputs1.setFillColor(sf::Color::Black);
    sf::Text outputs2("", font, 30);
    outputs2.setFillColor(sf::Color::Black);
    
    // a bool variable to check if results were outputted to window
    bool complete = false;
    
    // time execution locations for each of the data structures
    sf::Text dataTime1("Time Execution: ", font, 30);
    sf::Text dataTime2("Time Execution: ", font, 30);
    dataTime1.setPosition(150, 900);
    dataTime2.setPosition(900, 900);
    sf::Text rbTimeExec("", font, 30);
    sf::Text heapTimeExec("", font, 30);
    rbTimeExec.setPosition(350, 900);
    heapTimeExec.setPosition(1150, 900);
    
    // ----------------------------------- read in database ----------------------------------- //
    string path = "/Users/pipertaylor/Downloads/RAW_recipes.csv";
    unordered_map<string, set<string>> recipes; // map to store all recipe names & ingredients
    recipes = ReadFile(path);
    
    // long long due to overflow with smaller types but when using seconds comes out as 0
    long long rbTime = 0;
    long long heapTime = 0;
    
    vector<pair<string, int>> top16Tree;
    vector<pair<string, int>> top16Heap;
    
    //------------------------------ Check the window for events -------------------------------//
    // Start the window loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            
            // if user clicks specified box in window - allow the user to type
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)
               && sf::Mouse::getPosition(window).x >= 225
               && sf::Mouse::getPosition(window).x <= 1300
               && sf::Mouse::getPosition(window).y >= 150
               && sf::Mouse::getPosition(window).y <= 200) {
                text1.setSelected(true);
            }
            
            // if the return key is pressed, set recipe input
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                text1.setSelected(false);
                recipeInput = text1.getText();
                
                // display drop down list of all options the user can choose from
                dropDownOptions = searchFunction(recipeInput, recipes);
                cout << dropDownOptions.size() << endl;
                dropDownEnable = true;
                refresh = false;
                counter = 0;
            // when the user clicks on the window, logs keys pressed and outputs to window
            } else if(event.type == sf::Event::TextEntered) {
                text1.typedOn(event);
                recipeInput = text1.getText();
            }
            
            // if the drop down is enabled by an enter and click on drop down
            if (dropDownEnable && !displayOptions && sf::Mouse::isButtonPressed(sf::Mouse::Left)
                && sf::Mouse::getPosition(window).x >= 330
                && sf::Mouse::getPosition(window).x <= 1080
                && sf::Mouse::getPosition(window).y >= 250
                && sf::Mouse::getPosition(window).y <= 300) {
                displayOptions = true;
            // if the user selects the drop down again it will close the display Options
            } else if (displayOptions && sf::Mouse::isButtonPressed(sf::Mouse::Left)
                && sf::Mouse::getPosition(window).x >= 330
                && sf::Mouse::getPosition(window).x <= 1080
                && sf::Mouse::getPosition(window).y >= 250
                && sf::Mouse::getPosition(window).y <= 300) {
                displayOptions = false;
            }
            
            // if the user selects the refresh button
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
                && sf::Mouse::getPosition(window).x >= 1100
                && sf::Mouse::getPosition(window).x <= 1250
                && sf::Mouse::getPosition(window).y >= 250
                && sf::Mouse::getPosition(window).y <= 300) {
                refresh = true;
                counter++;
            }
            
            // determine the size of the drop down menu to determine area of clickability
            if (remainder == 0) {
                dimensions = 15;
            } else {
                dimensions = remainder;
            }
            
            // if the user selects within the option field for a certain recipe
            if (displayOptions && sf::Mouse::isButtonPressed(sf::Mouse::Left)
                && sf::Mouse::getPosition(window).x >= 380
                && sf::Mouse::getPosition(window).x <= 1250
                && sf::Mouse::getPosition(window).y >= 300
                && sf::Mouse::getPosition(window).y <= 300 + (30*dimensions+20)) {
                // find the exact area the user clicked within the options displayed
                int ypos = sf::Mouse::getPosition(window).y;
                int index = ((ypos - 300) / 30) + (15 * counter);
                recipeChosen = dropDownOptions[index];
                displayOptions = false;
                complete = false;
            }
        }

        // Clear screen
        window.clear();

        // Draw the strings
        window.draw(inputBox);
        window.draw(text);
        window.draw(descript);
        window.draw(chooseRecipe);
        window.draw(dropDownBox);
        window.draw(data1);
        window.draw(data2);
        
        // data structure 1 time
        window.draw(dataTitle1);
        window.draw(dataTime1);
        // data structure 2 time
        window.draw(dataTitle2);
        window.draw(dataTime2);
        
        if (recipeChosen != "" && dropDownOptions.size() != 0) {
            dropDownRecipe.setString(recipeChosen);
            window.draw(dropDownRecipe);
        }
        
        // if a recipe has been chosen, display the results from each of the data structures
        if (recipeChosen != "" && !complete) {
            RedBlackTree redBlack;
            MaxHeap maxHeap;
            
            top16Tree.clear();
            top16Heap.clear();
            
            set<string> ingredients;
            auto it = recipes.find(recipeChosen);
            if (it != recipes.end()) {
                ingredients = it->second; // find list of ingredients to be compared
                recipes.erase(it); // don't need searched recipe in results
            }
            unordered_map<string, int> matchingRecipes;

            for(auto member: recipes) { // find intersections of ingredient lists
                set<string>::iterator it1;
                matchingRecipes[member.first] = 0;
                for (it1 = member.second.begin(); it1 != member.second.end(); it1++) {
                    if (ingredients.find(*it1) != ingredients.end()) {
                        matchingRecipes[member.first]++;
                    }
                }
            }

            // insert recipes into tree and heap and time them
            // timer code from https://www.techiedelight.com/measure-elapsed-time-program-chrono-library/
            for (auto member : matchingRecipes) {
                string name = member.first;
                int num = member.second;
                pair<string, int> heap = make_pair(name, num);

                auto start = chrono::steady_clock::now();
                maxHeap.Insert(heap);
                auto end = chrono::steady_clock::now();
                heapTime += chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                start = chrono::steady_clock::now();
                redBlack.insert(name, num);
                end = chrono::steady_clock::now();
                rbTime += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            }
            
            // output matching recipes
            int count = 0;
            redBlack.printInorder(redBlack.getRoot(), count, top16Tree);
            maxHeap.PrintHeap(top16Heap);
            
            complete = true;
        }
        
        if (complete) {
            // print tree results
            if (top16Tree.size() < 16) {
                for (int i = 0; i < top16Tree.size(); i++) {
                    string temp = top16Tree[i].first + ": " + to_string(top16Tree[i].second);
                    outputs1.setString(temp);
                    outputs1.setPosition(25, 400 + (i*30));
                    window.draw(outputs1);
                }
            } else {
                for (int i = 0; i < 16; i++) {
                    // output results for red black tree
                    string temp = top16Tree[i].first + ": " + to_string(top16Tree[i].second);
                    outputs1.setString(temp);
                    outputs1.setPosition(25, 400 + (i*30));
                    window.draw(outputs1);
                }
            }
            dataTime1.setString("Time Execution: " + to_string(rbTime) + " (ns)");
            window.draw(dataTime1);
            
            // print heap results
            if (top16Heap.size() < 16) {
                for (int i = 0; i < top16Heap.size(); i++) {
                    string temp = top16Heap[i].first + ": " + to_string(top16Heap[i].second);
                    outputs2.setString(temp);
                    outputs2.setPosition(775, 400 + (i*30));
                    window.draw(outputs2);
                }
            } else {
                for (int i = 0; i < 16; i++) {
                    // output results for red black tree
                    string temp = top16Heap[i].first + ": " + to_string(top16Heap[i].second);
                    outputs2.setString(temp);
                    outputs2.setPosition(775, 400 + (i*30));
                    window.draw(outputs2);
                }
            }
            dataTime2.setString("Time Execution: " + to_string(heapTime) + " (ns)");
            window.draw(dataTime2);
        }
        
        // if there is no found matches
        if (dropDownOptions.size() == 0) {
            optionChoice.setString("No found results in database...");
            optionChoice.setPosition(400, 255);
            window.draw(optionChoice);
        } else if (displayOptions) {
            // if found matches are above 15
            if (dropDownOptions.size() > 15) {
                window.draw(refreshButton);
                window.draw(refreshTitle);
                // refresh the list to a new set of options for the user to select from
                // if they keep selecting refresh, it will loop back to the beginning of the set
                if (refresh) {
                    division = dropDownOptions.size() / 15;
                    remainder = dropDownOptions.size() % 15;
                    if (remainder == 0) {
                        options.setSize(sf::Vector2f(700, (30*15)+20));
                        window.draw(options);
                        
                        for(int i = 0; i < 15; i++) {
                            optionChoice.setString(dropDownOptions[i+(counter*15)]);
                            optionChoice.setPosition(400, 300 + (i*30));
                            window.draw(optionChoice);
                        }
                    } else if (division == counter && remainder != 0) {
                        options.setSize(sf::Vector2f(700, (30*remainder)+20));
                        window.draw(options);
                        
                        for(int i = remainder; i > 0; i--) {
                            optionChoice.setString(dropDownOptions[dropDownOptions.size() - i]);
                            optionChoice.setPosition(400, 300 + ((remainder - i)*30));
                            window.draw(optionChoice);
                        }
                    } else if (division != counter) {
                        if (division < counter) {
                            refresh = false;
                            counter = 0;
                        } else {
                            options.setSize(sf::Vector2f(700, (30*15)+20));
                            window.draw(options);
                            
                            for(int i = 0; i < 15; i++) {
                                optionChoice.setString(dropDownOptions[i+((counter)*15)]);
                                optionChoice.setPosition(400, 300 + (i*30));
                                window.draw(optionChoice);
                            }
                        }
                    }
                } else {
                    options.setSize(sf::Vector2f(700, (30*15)+20));
                    window.draw(options);
                    
                    for(int i = 0; i < 15; i++) {
                        optionChoice.setString(dropDownOptions[i]);
                        optionChoice.setPosition(400, 300 + (i*30));
                        window.draw(optionChoice);
                    }
                }
            // if found matches are within 15
            } else if (dropDownOptions.size() < 15 && dropDownOptions.size() >= 1){
                options.setSize(sf::Vector2f(700, (30*(dropDownOptions.size()-1))+20));
                window.draw(options);
                for (int i = 0; i < dropDownOptions.size(); i++) {
                    optionChoice.setString(dropDownOptions[i]);
                    optionChoice.setPosition(400, 300 + (i*30));
                    window.draw(optionChoice);
                }
            }
        } else if (dropDownOptions.size() > 15) {
            window.draw(refreshButton);
            window.draw(refreshTitle);
        }
    
        text1.drawTo(window);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}

