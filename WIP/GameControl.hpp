#ifndef _GAMECONTROL_HPP
#define _GAMECONTROL_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include "Objects.hpp"

//Class where the player's information is stored
class playerStats{
public:
    int shipSpeed = 5;
};

//Action class which provides usefull functions
class action {
private:
	std::function< bool() > condition;
	std::function< void() > work;
public:
	action(
	   std::function< bool() > condition, 
	   std::function< void() > work
	) : condition( condition ), 
		work( work ) 
	{}

	action(
		sf::Keyboard::Key key,
		std::function< void() > work
	) :
		condition(
			[ key ]()->bool { return sf::Keyboard::isKeyPressed( key ); }
		),
		work(work)
	{}

	action(
		std::function< void() > work
	) :
		condition(
			[]()->bool { return true; }
		),
		work(work)
	{}

	void operator()(){
		if( condition() ){
			work();
		}
	}
};

//Class which provides the central game loop
class gameLoop{
private:
    playerStats stats;
    sf::Time updateTime = sf::milliseconds(20);
    sf::RenderWindow window{ sf::VideoMode{ 1920, 875 },"SFML window" };

    std::map<std::string, rectangle> borderList = {
        {"wall_top",    rectangle( sf::Vector2f( 394.0, 0.0 ),   sf::Color(0,0,0), sf::Vector2f( 1142.0, 10.0 ) )},
        {"wall_bottom", rectangle( sf::Vector2f( 384.0, 865.0 ), sf::Color(0,0,0), sf::Vector2f( 1152.0, 10.0 ) )},
        {"wall_right",  rectangle( sf::Vector2f( 1536.0, 0.0 ),  sf::Color(255,255,255), sf::Vector2f( 10.0, 1080.0 ) )},
        {"wall_left",   rectangle( sf::Vector2f( 384.0, 0.0 ),   sf::Color(255,255,255), sf::Vector2f( 10.0, 1080.0 ) )}
    };
    std::map<std::string, picture> objectList = {
        {"ship", picture(sf::Vector2f( 500.0, 500.0 ), "images/ship1.png", sf::Vector2f( 80.0f, 80.0f ))}
    };

    action actions[4] = {
        action( sf::Keyboard::Left,  [&](){ 
            objectList["ship"].move( sf::Vector2f( -stats.shipSpeed, 0.0 )); 
            if(objectList["ship"].collision( borderList["wall_left"] )){
                objectList["ship"].move( sf::Vector2f( stats.shipSpeed, 0.0 )); 
            }
        }),
        action( sf::Keyboard::Right, [&](){ 
            objectList["ship"].move( sf::Vector2f( stats.shipSpeed, 0.0 ));
            if(objectList["ship"].collision( borderList["wall_right"] )){
                objectList["ship"].move( sf::Vector2f( -stats.shipSpeed,  0.0 )); 
            } 
        }),
        action( sf::Keyboard::Up,    [&](){ 
            objectList["ship"].move( sf::Vector2f( 0.0, -stats.shipSpeed )); 
            if(objectList["ship"].collision( borderList["wall_top"] )){
                objectList["ship"].move( sf::Vector2f( 0.0, stats.shipSpeed )); 
            }
        }),
        action( sf::Keyboard::Down,  [&](){ 
            objectList["ship"].move( sf::Vector2f( 0.0, stats.shipSpeed )); 
            if(objectList["ship"].collision( borderList["wall_bottom"] )){
                objectList["ship"].move( sf::Vector2f( 0.0, -stats.shipSpeed )); 
            }
        })
    };

public:
    void loop(){
        sf::Time lag = sf::milliseconds(0);
        sf::Clock clock;

        while (true){
            sf::sleep( sf::milliseconds(20));

            sf::Time elapsed = clock.getElapsedTime();
            lag += elapsed;

            processInput();
            while (lag >= updateTime){
                update();
                lag -= updateTime;
            }

            render();
            clock.restart();
        }
    }

    void processInput(){
        sf::Event event;
        while( window.pollEvent(event) ){
            if( event.type == sf::Event::Closed ){
                window.close();
            }
        }
        for( auto & action : actions ){
            action();
        }
    }

    void update(){
        return;
    }

    void render(){
        window.clear();

        for(auto & picture: objectList){
            picture.second.draw( window );
        }
        for(auto & border: borderList){
            border.second.draw( window );
        }

        window.display();
    }
};


#endif
