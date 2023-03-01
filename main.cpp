/*
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023
*/


#include <SFML/Graphics.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include "TemplateMap.h"
#include "fillTemplateMap.h"
#include "recognizer.h"
#include "parseXML.h"
using namespace std;


int main()
{
    using boost::property_tree::ptree;  
    /*
    // boost sanity check (test this after installing boost library by typing "1 2 3")
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " ");*/

        // ============== REMOVE COMMENT BLOCK FOR PART 3 =============
    /*
        vector<TemplateMap> rawUserData;
        parseXML(rawUserData);
        //FILL FROM XML HERE

        // Preprocess points from XML file and save to preprocessedTemplates data structure
        vector<TemplateMap> preprocessedUserData;
        //for each user
        for (unsigned int i = 0; i < 1; i++) {
            //for each gesture type in map
            TemplateMap processed;
            for (auto itr = rawUserData[i].templates.begin(); itr != rawUserData[i].templates.end(); itr++) {
                for (unsigned int j = 0; j < itr->second.size(); j++) {
                    //cout << "preprocessing" << endl;
                    vector<Point> resampled;
                    resample(itr->second[j], 64, resampled);
                    //Rotation function calls
                    vector<Point> rotated;
                    rotateToZero(resampled, 64, rotated);
                    //Scaling & translation function calls
                    vector<Point> scaled;
                    scaled = ScaleTo(rotated, 400);
                    vector<Point> translated;
                    translated = TranslateTo(scaled, Point(200, 200));
                    //Add the template to the templatemap for the current user
                    processed.addTemplate(itr->first, translated);
                }
            }
            preprocessedUserData.push_back(processed);
        }

        // LOOP OVER DATASET, OUTPUT THE RESULT
        vector<vector<double>> recoScores(rawUserData.size(), vector<double>(rawUserData[0].templates.size(), 0));
        vector<pair<long, long>> userAvg;
        vector<vector<string>> outputCSV;

        // for each user U = 1 to 10
        for (unsigned int U = 0; U < 1; U++) {
            userAvg.push_back(pair<long, long>(0, 0));

            // for each example E = 1 to 9
            for (int E = 1; E < 10; E++) {

                for (int i = 0; i < 10; i++) {
                    TemplateMap chosenGestureTemplates;
                    vector<pair<vector<Point>, int>> candidates;
                    string setContents = "{";
                    vector<string> namesInOrder;

                    // for each gesture type G
                    for (auto G = rawUserData[U].templates.begin(); G != rawUserData[U].templates.end(); G++) {
                        TemplateMap randTemplates;
                        vector<vector<Point>> gestures = preprocessedUserData.at(U).templates[G->first];
                        vector<bool> alreadyChose;
                        alreadyChose.resize(gestures.size(), false);

                        // Choose E templates and 1 candidate
                        for (int j = 0; j <= E; j++) {
                            int randInd = rand() % gestures.size();

                            if (!alreadyChose[randInd]) {
                                if (j < E) {
                                    chosenGestureTemplates.addTemplate(G->first, gestures[randInd]);
                                    setContents += "s0" + to_string(U + 2) + "-" + G->first + "-" + to_string(randInd) + " ";
                                    namesInOrder.push_back("s0" + to_string(U + 2) + "-" + G->first + "-" + to_string(randInd));

                                }
                                else {
                                    // last iteration, this is the candidate
                                    candidates.push_back(pair<vector<Point>, int>(gestures[randInd], randInd));

                                }

                            }
                            else {
                                j--;
                            }
                        }
                    }

                    int g = 0;
                    // for each candidate T (templates[E]) recognize with E (templates[0 to E])
                    for (auto G = rawUserData[U].templates.begin(); G != rawUserData[U].templates.end(); G++) {
                        // prepare a vector of strings for csv output
                        map<double, string> nBest;
                        string participant = "s0" + to_string(U + 2) + "-";
                        string templateName = G->first;

                        pair<string, double> result = Recognize(candidates[g].first, chosenGestureTemplates, nBest, namesInOrder);
                        //cout << "n best size " << nBest.size() << endl;

                        vector<string> log;
                        log.push_back("s0" + to_string(U + 2)); // log user
                        log.push_back(G->first); // log gesture
                        log.push_back(to_string(i)); // log iteration
                        log.push_back(to_string(E)); // log num examples
                        log.push_back(to_string(rawUserData.size())); // log size of training set
                        log.push_back(setContents.substr(0, setContents.size() - 1) + "}"); // log the contents of the set
                        log.push_back("s0" + to_string(U + 2) + "-" + G->first + to_string(candidates[g].second)); // log the candidate
                        log.push_back(result.first); // log the result of the recognizer

                        // reco score for each U,G += 1
                        if ((G->first).compare(result.first) == 0) {
                            recoScores[U][g]++;
                            userAvg[U].first++;
                            log.push_back("1"); // log success

                        } else {
                            userAvg[U].second++;
                            log.push_back("0"); // log fail

                        }

                        log.push_back(to_string(result.second)); // log recognizer score

                // CODE FOR ADDING NAME OF GESTURE RECOGNIZED

                        vector<string> contentsOfSet;
                        string s = setContents.substr(1, contentsOfSet.size()-1);
                        boost::split(contentsOfSet, s, boost::is_any_of(" "), boost::token_compress_on);
                        //cout << contentsOfSet[0] << endl;

                        string recognizedAs;

                        for (int i = 0; i < contentsOfSet.size(); i++) {
                            vector<string> parts;
                            string gesture = contentsOfSet[i];
                            boost::split(parts, gesture, boost::is_any_of("-"), boost::token_compress_on);
                            string j = parts[1];
                            if (j == result.first) {
                                recognizedAs = "s0" + to_string(U+2) + "-" + j + "-" + parts[2];
                                //cout << "R as: " << recognizedAs << endl;
                                log.push_back(recognizedAs); // log what gesture was recognized as
                                break;
                            }
                        }

                        string nBestString = "{";
                        int i = 0;
                        for (auto itr = nBest.begin(); itr != nBest.end(); itr++) {
                            if (i < 50) {
                                nBestString += itr->second;
                                nBestString += "|";
                                nBestString += to_string(itr->first);
                                nBestString += "|";
                            }
                            i++;
                        }
                        nBestString += "}";
                        log.push_back(nBestString);

                        g++;

                        // add log to output CVS file
                        outputCSV.push_back(log);
                    }
                }

                // reco score for each U,G /= 10
                for (unsigned int i = 0; i < rawUserData[U].templates.size(); i++) {
                    recoScores[U][i] /= 10.0;
                }
            }

            double average = ((double)userAvg[U].first) / (userAvg[U].first + userAvg[U].second);
            cout << "User " << U + 1 << " accuracy: " << average << endl;
            vector<string> empty;
            //empty line
            outputCSV.push_back(empty);
            vector<string> final;
            final.push_back("TotalAvgAccuracy");
            final.push_back(to_string(average));
            outputCSV.push_back(final);
        }

        // write csv file
        ofstream myfile;
        myfile.open("output.csv");
        myfile << "User[all-users],GestureType[all-gestures-types],RandomIteration[1to100],#ofTrainingExamples[E],TotalSizeOfTrainingSet[count],TrainingSetContents[specific-gesture-instances],Candidate[specific-instance],RecoResultGestureType[what-was-recognized],CorrectIncorrect[1or0],RecoResultScore,RecoResultBestMatch[specific-instance],RecoResultNBestSorted[instance-and-score]\n";
        for (int i = 0; i < outputCSV.size(); i++) {
            for (int j = 0; j < outputCSV.at(i).size(); j++) {
                myfile << outputCSV.at(i).at(j) + ",";
            }
            myfile << "\n";
        }
        myfile.close();

        return 0;
    }

*/
	//fillTemplateMap(templateMap, preprocessedTemplates);
    //templateMap.printTemplateMap();
    
    
    int width = 800;
    int height = 450;

    sf::RenderWindow window(sf::VideoMode(width, height), "Canvas");
    sf::Texture clearBtn;
    sf::Texture clearBtnPressed;
    sf::Texture enterBtn;
    sf::Texture enterBtnPressed;
    sf::Texture gestureExamples;
    sf::Sprite clearBtnSprite;
    sf::Sprite enterBtnSprite;
    sf::Sprite gestureExamplesSprite;
    sf::Font outputFont;
    sf::Text inputText;
    sf::Text instructions;

    window.setFramerateLimit(1000);

    // load clear button states (pressed and unpressed)
    if (!clearBtn.loadFromFile("clear.png")) {
        std::cout << "failed to load clear.png";
    }

    if (!clearBtnPressed.loadFromFile("clear_pressed.png")) {
        std::cout << "failed to load clear_pressed.png";
    }

    if (!enterBtn.loadFromFile("enter.png")) {
        cout << "failed to load enter.png";
    }

    if (!enterBtnPressed.loadFromFile("enter_pressed.png")) {
        cout << "failed to load enter_pressed.png";
    }

    if (!outputFont.loadFromFile("Roboto-Black.ttf")) {
        std::cout << "failed to load Roboto-Black.ttf";
    }

    if (!gestureExamples.loadFromFile("gestures.jpg")) {
        std::cout << "failed to load gestures.jpg";
    }

    clearBtnSprite.setPosition(300, 400);
    clearBtnSprite.setTexture(clearBtn);

    enterBtnSprite.setPosition(0, 400);
    enterBtnSprite.setTexture(enterBtn);

    gestureExamplesSprite.setPosition(400, 50);
    gestureExamplesSprite.setTexture(gestureExamples);

    inputText.setPosition(10, 0);
    inputText.setFont(outputFont);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::Blue);
    inputText.setString("Test");

    instructions.setPosition(10, 30);
    instructions.setFont(outputFont);
    instructions.setCharacterSize(16);
    instructions.setFillColor(sf::Color::Blue);

    //tracks actual points drawn
    //sf::VertexArray vertices(sf::LineStrip);
    vector<sf::Vertex> vertices;
    vector<sf::Vertex> resampledVisualization;

    sf::Vector2f last;
    bool drawing = false;


    vector<string> gestures = { "arrow", "caret", "check", "circle", "delete mark", "left curly brace", "left square bracket", "pigtail", "rectangle", "right curly brace", "right square bracket", "star", "triangle", "v", "x", "zig-zag"};
    vector<string> randGestures;
    //setup for gesture prompts
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 16; j++) {
            randGestures.push_back(gestures[j]);
        }
    }
    srand(time(NULL));
    int currGesture = rand() % randGestures.size();

    TemplateMap userMap;
    vector<Point> shape;
    int numRecordings = 0;


    while (window.isOpen())
    {
        

        inputText.setString("Draw gesture: " + randGestures[currGesture]);
        instructions.setString("Draw the prompted gesture. Press enter to proceed to the next gesture, or press clear to try again.");


        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //visualize first point of contact
            else if (event.type == sf::Event::MouseButtonPressed) {
                // if pressed clear btn, do not draw
                if (event.mouseButton.x >= 300 && event.mouseButton.y >= 400) {
                    clearBtnSprite.setTexture(clearBtnPressed);
                }
                else if (event.mouseButton.x <= 100 && event.mouseButton.y >= 400) {
                    enterBtnSprite.setTexture(enterBtnPressed);
                }
                else {
                    //start new stroke
                    
                    vertices.clear();
                    drawing = true;
                    last = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                    vertices.push_back(sf::Vertex(last, sf::Color(0, 0, 0)));
                    break;
                }
            }
            //connect additional points of contact
            else if (event.type == sf::Event::MouseMoved && drawing) {
                if (event.mouseMove.x <= 400 && event.mouseMove.y > 50) {
                    const sf::Vector2f next(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
                    vertices.push_back(sf::Vertex(next, sf::Color(0, 0, 0)));
                    last = next;
                    break;
                }
                else {
                    drawing = false;
                }
            }
            //use enter to finish drawing
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Enter) {
                    //collect points - write to xml here
                    for (int i = 0; i < vertices.size(); i++) {
                        shape.push_back(Point(vertices[i].position.x, vertices[i].position.y));
                    }
                    cout << "Curr: " << currGesture << endl;
                    cout << "Size: " << randGestures.size() << endl;
                    cout << "Gesture: " << randGestures[currGesture] << endl;
                    userMap.addTemplate(randGestures[currGesture], shape);
                    //userMap.printTemplateMap();
                    //clear window points
                    clearBtnSprite.setTexture(clearBtn);
                    vertices.clear();
                    resampledVisualization.clear();
                    
                    inputText.setString("Draw gesture: " + randGestures[currGesture]);

                    numRecordings++;

                    if (numRecordings == 160) { // user has drawn 10 examples of all 16 gestures
                        // turn to xml, should be 160 just using 1 to test
                        cout << "starting conversion" << endl;
                        
                        for (int i = 0; i < gestures.size(); i++) {
                            vector<vector<Point>> temp = userMap.templates[gestures[i]]; // should be gestures[i]
                            
                            for (int j = 0; j < temp.size(); j++) {
                                
                                
                                //pt.add("<xmlattr>.Name", randGestures[currGesture]);

                                //ptree p;
                                //p.clear();

                                ptree pt;
                                pt.clear();

                                for (int k = 0; k < temp[j].size(); k++) {
                                     ptree ptt;
                                     ptt.clear();
                                     //ptree pts;
                                     ptt.add("<xmlattr>.X", temp[j][k].x);
                                     ptt.add("<xmlattr>.Y", temp[j][k].y);
                                     pt.add_child("Point", ptt);
                                }

                                string tempName = gestures[i];
                                vector<string> tempNameResult;
                                boost::split(tempNameResult, tempName, boost::is_any_of(" "));
                                string name = "";

                                if (tempNameResult.size() > 0) {
                                    for (int l = 0; l < tempNameResult.size(); l++) {
                                        if (l < tempNameResult.size() - 1) {
                                            name += tempNameResult[l] + "_";
                                        }
                                        else {
                                            name += tempNameResult[l];
                                        }
                                    }
                                }
                                else {
                                    name = tempName;
                                }

                                if (j + 1 < 10) {
                                    name += "0" + to_string(j + 1);
                                }
                                else {
                                    name += to_string(j + 1);
                                }
                                
                                //ptt.clear();
                                //ptt.add("<xmlattr>.Name", gestures[i]);
                                boost::property_tree::ptree pts;
                                pts.clear();
                                pt.put("<xmlattr>.Name", name);
                                pts.add_child("Gesture", pt);

                                

                                
                                
                                cout << name << endl;
                                cout << name + ".xml" << endl;
                                write_xml("xml/" + name + ".xml", pts);
                                cout << "done" << endl;
                            }
                            
                        }
                        exit(0);

                    }
                    else {
                        randGestures.erase(randGestures.begin() + currGesture);
                        if (randGestures.size() != 0) {
                            currGesture = rand() % randGestures.size();
                        }
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (drawing) {
                    
                    
                    drawing = false;
                    /*Point origin = Point(0, 0);
                    for (int i = 0; i < vertices.size(); i++) {
                        shape.push_back(Point(vertices[i].position.x, vertices[i].position.y));
                        //cout << "Test: Distance from origin: " << shape.at(i).distance(origin) << endl;
                        //cout << vertices[i].position.x << " " << vertices[i].position.y << endl;
                    }

                    ============ RECOGNIZING DATA ========= commented out for part 4
                    
                    //Resampling function calls
                    vector<Point> resampled;
                    cout << "Original points: " << shape.size() << endl;
                    resample(shape, 64, resampled);
                    cout << "Resampled Points: " << resampled.size() << endl;
                    //Rotation function calls
                    vector<Point> rotated;
                    rotateToZero(resampled, resampled.size(), rotated);
                    //Scaling & translation function calls
                    vector<Point> scaled;
                    scaled = ScaleTo(rotated, width);
                    vector<Point> translated;
                    translated = TranslateTo(scaled, Point(width / 2.0, height / 2.0));
                    //Recognize
                    outputText.setString(" " + Recognize(translated, preprocessedTemplates).first);
                    */

                    //visualize resamped and rotated and translated points for fun, comment out when not debugging
                    /*
                    for (int i = 0; i < rotated.size(); i++) {
                        resampledVisualization.push_back(sf::Vertex(sf::Vector2f(translated[i].x, translated[i].y), sf::Color(0, 0, 0)));
                        //cout << translated[i].x << " " << translated[i].y << endl;
                    }*/
                    // REMOVE COMMENT TO USE GUI break;
                //}

                // if mouse btn released outside of clear btn, do not clear 
                }
                else if (clearBtnSprite.getTexture() == &clearBtnPressed && (event.mouseButton.x < 300 || event.mouseButton.y < 400)) {
                    clearBtnSprite.setTexture(clearBtn);
                }
                else if (enterBtnSprite.getTexture() == &enterBtnPressed && (event.mouseButton.x > 100 || event.mouseButton.y < 400)) {
                    enterBtnSprite.setTexture(enterBtn);
                }
                // clear
                else if (clearBtnSprite.getTexture() == &clearBtnPressed && event.mouseButton.x >= 300 && event.mouseButton.y >= 400) {
                    
                    clearBtnSprite.setTexture(clearBtn);
                    vertices.clear();
                    resampledVisualization.clear();
                }
                else if (enterBtnSprite.getTexture() == &enterBtnPressed && event.mouseButton.x <= 100 && event.mouseButton.y >= 400) {
                    //collect points - write to xml here
                    for (int i = 0; i < vertices.size(); i++) {
                        shape.push_back(Point(vertices[i].position.x, vertices[i].position.y));
                    }
                    cout << "Curr: " << currGesture << endl;
                    cout << "Size: " << randGestures.size() << endl;
                    cout << "Gesture: " << randGestures[currGesture] << endl;
                    userMap.addTemplate(randGestures[currGesture], shape);
                    //userMap.printTemplateMap();
                    //clear window points
                    enterBtnSprite.setTexture(enterBtn);
                    clearBtnSprite.setTexture(clearBtn);
                    vertices.clear();
                    resampledVisualization.clear();

                    inputText.setString("Draw gesture: " + randGestures[currGesture]);

                    numRecordings++;

                    if (numRecordings == 160) { // user has drawn 10 examples of all 16 gestures
                        // turn to xml, should be 160 just using 1 to test
                        cout << "starting conversion" << endl;

                        for (int i = 0; i < gestures.size(); i++) {
                            vector<vector<Point>> temp = userMap.templates[gestures[i]]; // should be gestures[i]

                            for (int j = 0; j < temp.size(); j++) {


                                //pt.add("<xmlattr>.Name", randGestures[currGesture]);

                                //ptree p;
                                //p.clear();

                                ptree pt;
                                pt.clear();

                                for (int k = 0; k < temp[j].size(); k++) {
                                    ptree ptt;
                                    ptt.clear();
                                    //ptree pts;
                                    ptt.add("<xmlattr>.X", temp[j][k].x);
                                    ptt.add("<xmlattr>.Y", temp[j][k].y);
                                    pt.add_child("Point", ptt);
                                }

                                string tempName = gestures[i];
                                vector<string> tempNameResult;
                                boost::split(tempNameResult, tempName, boost::is_any_of(" "));
                                string name = "";

                                if (tempNameResult.size() > 0) {
                                    for (int l = 0; l < tempNameResult.size(); l++) {
                                        if (l < tempNameResult.size() - 1) {
                                            name += tempNameResult[l] + "_";
                                        }
                                        else {
                                            name += tempNameResult[l];
                                        }
                                    }
                                }
                                else {
                                    name = tempName;
                                }

                                if (j + 1 < 10) {
                                    name += "0" + to_string(j + 1);
                                }
                                else {
                                    name += to_string(j + 1);
                                }

                                //ptt.clear();
                                //ptt.add("<xmlattr>.Name", gestures[i]);
                                boost::property_tree::ptree pts;
                                pts.clear();
                                pt.put("<xmlattr>.Name", name);
                                pts.add_child("Gesture", pt);





                                cout << name << endl;
                                cout << name + ".xml" << endl;
                                write_xml("xml/" + name + ".xml", pts);
                                cout << "done" << endl;
                            }

                        }
                        exit(0);
                    }
                    else {
                        randGestures.erase(randGestures.begin() + currGesture);
                        if (randGestures.size() != 0) {
                            currGesture = rand() % randGestures.size();
                        }
                    }

                    
                }
            }
            
        }

        window.clear(sf::Color(255,255,255));
        window.draw(clearBtnSprite);
        window.draw(enterBtnSprite);
        window.draw(gestureExamplesSprite);
        window.draw(inputText);
        window.draw(instructions);

        //draw only actual points recorded and lines in between
        if (!vertices.empty() && resampledVisualization.empty()) {
            resampledVisualization.clear();
            shape.clear();
            window.draw(&vertices[0], vertices.size() - 1, sf::LineStrip);
        }
        
        //comment out when not debugging
        if (!resampledVisualization.empty()) {
            window.draw(&resampledVisualization[0], resampledVisualization.size() - 1, sf::Points);
        }
        window.display();

    }

    return 0;

    
}
