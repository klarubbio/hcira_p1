/*
Code written by Evan Brooks, Karina LaRubbio, and Trevor Richardson
CIS 4930 - Human-Centered Input Recognition Algorithms at the University of Florida
Spring 2023
*/


#include <SFML/Graphics.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <fstream>
#include "TemplateMap.h"
#include "fillTemplateMap.h"
#include "recognizer.h"
#include "parseXML.h"
using namespace std;


int main()
{
    /*
    // boost sanity check (test this after installing boost library by typing "1 2 3")
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " ");*/
    vector<TemplateMap> rawUserData;
    //parseXML(rawUserData);
    //FILL FROM XML HERE

    // Preprocess points from XML file and save to preprocessedTemplates data structure
    vector<TemplateMap> preprocessedUserData;
    //for each user
    for (unsigned int i = 0; i < 10; i++) {
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
    for (unsigned int U = 0; U < 10; U++) {
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

    /*REMOVE COMMENT TO USE GUI
    vector<Point> shape;
	fillTemplateMap(templateMap, preprocessedTemplates);
    //templateMap.printTemplateMap();
    
    
    int width = 400;
    int height = 400;
    sf::RenderWindow window(sf::VideoMode(width, height), "Canvas");
    sf::Texture clearBtn;
    sf::Texture clearBtnPressed;
    sf::Sprite clearBtnSprite;
    sf::Font outputFont;
    sf::Text outputText;

    window.setFramerateLimit(1000);

    // load clear button states (pressed and unpressed)
    if (!clearBtn.loadFromFile("clear.png")) {
        std::cout << "failed to load clear.png";
    }

    if (!clearBtnPressed.loadFromFile("clear_pressed.png")) {
        std::cout << "failed to load clear_pressed.png";
    }

    if (!outputFont.loadFromFile("Roboto-Black.ttf")) {
        std::cout << "failed to load Roboto-Black.ttf";
    }

    clearBtnSprite.setPosition(300, 350);
    clearBtnSprite.setTexture(clearBtn);

    outputText.setPosition(0, 375);
    outputText.setFont(outputFont);
    outputText.setCharacterSize(24);
    outputText.setFillColor(sf::Color::Blue);

    //tracks actual points drawn
    //sf::VertexArray vertices(sf::LineStrip);
    vector<sf::Vertex> vertices;
    vector<sf::Vertex> resampledVisualization;

    sf::Vector2f last;
    bool drawing = false;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //visualize first point of contact
            else if (event.type == sf::Event::MouseButtonPressed) {
                // if pressed clear btn, do not draw
                if (event.mouseButton.x >= 300 && event.mouseButton.y >= 350) {
                    clearBtnSprite.setTexture(clearBtnPressed);
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
                const sf::Vector2f next(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
                vertices.push_back(sf::Vertex(next, sf::Color(0, 0, 0)));
                last = next;
                break;
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (drawing) {
                    drawing = false;
                    Point origin = Point(0, 0);
                    for (int i = 0; i < vertices.size(); i++) {
                        shape.push_back(Point(vertices[i].position.x, vertices[i].position.y));
                        //cout << "Test: Distance from origin: " << shape.at(i).distance(origin) << endl;
                        //cout << vertices[i].position.x << " " << vertices[i].position.y << endl;
                    }

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
                    translated = TranslateTo(scaled, Point(width/2.0,height/2.0));
                    //Recognize
                    outputText.setString(" " + Recognize(translated, preprocessedTemplates).first);


                    //visualize resamped and rotated and translated points for fun, comment out when not debugging
                    /*
                    for (int i = 0; i < rotated.size(); i++) {
                        resampledVisualization.push_back(sf::Vertex(sf::Vector2f(translated[i].x, translated[i].y), sf::Color(0, 0, 0)));
                        //cout << translated[i].x << " " << translated[i].y << endl;
                    }*/
                    // REMOVE COMMENT TO USE GUI break;
                //}
                // if mouse btn released outside of clear btn, do not clear
                /* REMOVE COMMENT TO USE GUI
                else if (clearBtnSprite.getTexture() == &clearBtnPressed && (event.mouseButton.x < 300 || event.mouseButton.y < 350)) {
                    clearBtnSprite.setTexture(clearBtn);;
                }
                // clear
                else if (clearBtnSprite.getTexture() == &clearBtnPressed && event.mouseButton.x >= 300 && event.mouseButton.y >= 350) {
                    clearBtnSprite.setTexture(clearBtn);
                    vertices.clear();
                    resampledVisualization.clear();
                }
            }
            
        }

        window.clear(sf::Color(255,255,255));
        window.draw(clearBtnSprite);
        window.draw(outputText);
        //draw only actual points recorded and lines in between
        if (!vertices.empty() && resampledVisualization.empty()) {
            resampledVisualization.clear();
            shape.clear();
            window.draw(&vertices[0], vertices.size() - 1, sf::LineStrip);
        }
        /*
        //comment out when not debugging
        if (!resampledVisualization.empty()) {
            window.draw(&resampledVisualization[0], resampledVisualization.size() - 1, sf::Points);
        }*/ /* REMOVE COMMENT TO USE GUI
        window.display();

    }
    
                return 0;

    
}*/
