------------------------------------------------------------ 
----- functions you can use -------------------------------- 
------------------------------------------------------------ 

--setemotion(emotion, settoletter)
--say(fontsize, textspeed, question, whattosay)
--popupquestions(one, two, three, four, five, six, seven)
--getNeighborName()
--getItemName()
--getPlayerName()

-----------------------------------------------------------

--table that contains the dialogue
dialogue = {}
dialogue['Holiday'] = {}
dialogue['Holiday']['Thanksgiving'] = {}
dialogue['Holiday']['Thanksgiving']['size'] = 1
dialogue['Holiday']['Christmas Eve'] = {}
dialogue['Holiday']['Christmas Eve']['size'] = 1
dialogue['Holiday']['Christmas'] = {}
dialogue['Holiday']['Christmas']['size'] = 1
dialogue['Holiday']['New Years Eve'] = {}
dialogue['Holiday']['New Years Eve']['size'] = 1
dialogue['Holiday']['New Years'] = {}
dialogue['Holiday']['New Years']['size'] = 1
dialogue['Holiday']['Valentines Day'] = {}
dialogue['Holiday']['Valentines Day']['size'] = 1
dialogue['Holiday']['Halloween'] = {}
dialogue['Holiday']['Halloween']['size'] = 1
dialogue['Holiday']['Easter'] = {}
dialogue['Holiday']['Easter']['size'] = 1
dialogue['Mood'] = {}
dialogue['Mood']['Happy'] = {}
dialogue['Mood']['Happy']['size'] = 1
dialogue['Mood']['Neutral'] = {}
dialogue['Mood']['Neutral']['size'] = 1
dialogue['Mood']['Sad'] = {}
dialogue['Mood']['Sad']['size'] = 1
dialogue['Mood']['Mad'] = {}
dialogue['Mood']['Mad']['size'] = 1
dialogue['TimeOfDay'] = {}
dialogue['TimeOfDay']['Morning'] = {}
dialogue['TimeOfDay']['Morning']['size'] = 1
dialogue['TimeOfDay']['Afternoon'] = {}
dialogue['TimeOfDay']['Afternoon']['size'] = 1
dialogue['TimeOfDay']['Evening'] = {}
dialogue['TimeOfDay']['Evening']['size'] = 1
dialogue['TimeOfDay']['Overnight'] = {}
dialogue['TimeOfDay']['Overnight']['size'] = 1
dialogue['Greeting'] = {}
dialogue['Greeting']['Morning'] = {}
dialogue['Greeting']['Morning']['size'] = 1
dialogue['Greeting']['Afternoon'] = {}
dialogue['Greeting']['Afternoon']['size'] = 1
dialogue['Greeting']['Evening'] = {}
dialogue['Greeting']['Evening']['size'] = 1
dialogue['Greeting']['Overnight'] = {}
dialogue['Greeting']['Overnight']['size'] = 1
dialogue['Greeting']['Rain'] = {}
dialogue['Greeting']['Rain']['size'] = 1
dialogue['First Time'] = {}
dialogue['First Time']['First Time'] = {}
dialogue['First Time']['First Time']['size'] = 1

function talk()

 say(0.4, 20, false, "Well happy thanksgiving") 
 finish()
end

dialogue['Holiday']['Thanksgiving'][1] = talk

function talk()

 say(0.4, 20, false, "Merry Christmas Eve.") 
 finish()
end

dialogue['Holiday']['Christmas Eve'][1] = talk

function talk()

 say(0.4, 20, false, "Merry Christmas") 
 finish()
end

dialogue['Holiday']['Christmas'][1] = talk

function talk()

 say(0.4, 20, false, "Happy New Years eve.") 
 finish()
end

dialogue['Holiday']['New Years Eve'][1] = talk

function talk()

 say(0.5, 20, false, "Well there, happy new years.  New year, new me right?") 
 finish()
end

dialogue['Holiday']['New Years'][1] = talk

function talk()

 say(0.4, 20, false, "Oh my, it is Valentine's Day isn't it.  I wonder who has a crush on me.") 
 finish()
end

dialogue['Holiday']['Valentines Day'][1] = talk

function talk()

 say(0.4, 20, false, "Hey there, happy Halloween.") 
 finish()
end

dialogue['Holiday']['Halloween'][1] = talk

function talk()

 say(0.4, 20, false, "Happy Easter or whatever") 
 finish()
end

dialogue['Holiday']['Easter'][1] = talk

function talk()

 say(0.4, 20, false, "I'm excited about everything!") 
 finish()
end

dialogue['Mood']['Happy'][1] = talk

function talk()
 setEmotion(0, 5)
 say(0.4, 20, false, "Well hey there.") 
 finish()
end

dialogue['Mood']['Neutral'][1] = talk

function talk()

 say(0.4, 20, false, "I hate my life.") 
 finish()
end

dialogue['Mood']['Sad'][1] = talk

function talk()

 say(0.4, 20, false, "What the hell!") 
 finish()
end

dialogue['Mood']['Mad'][1] = talk

function talk()

 say(0.4, 20, false, "It is days like this") 
 finish()
end

dialogue['TimeOfDay']['Morning'][1] = talk

function talk()

 say(0.4, 20, false, "this afternoon, something something something something something") 
 finish()
end

dialogue['TimeOfDay']['Afternoon'][1] = talk

function talk()

 say(0.4, 20, false, "I always love going on evening walks.") 
 finish()
end

dialogue['TimeOfDay']['Evening'][1] = talk

function talk()

 say(0.4, 20, false, "So, what are you doing up this late?") 
 finish()
end

dialogue['TimeOfDay']['Overnight'][1] = talk

function talk()

 say(0.4, 20, false, "Good morning.") 
 finish()
end

dialogue['Greeting']['Morning'][1] = talk

function talk()

 say(0.4, 20, false, "Good afternoon.") 
 finish()
end

dialogue['Greeting']['Afternoon'][1] = talk

function talk()

 say(0.4, 20, false, "Good evening.") 
 finish()
end

dialogue['Greeting']['Evening'][1] = talk

function talk()

 say(0.4, 20, false, "Well hey there, what are you doing on a night like this?") 
 finish()
end

dialogue['Greeting']['Overnight'][1] = talk

function talk()

 say(0.4, 20, false, "What are you doing wandering around in the rain like this?") 
 finish()
end

dialogue['Greeting']['Rain'][1] = talk

function talk()

 say(0.4, 20, false, "Well hey there, nice to meet you.  My name is (Neighbor)") 
 finish()
end

dialogue['First Time']['First Time'][1] = talk

dialogueTable['Roxy'] = dialogue