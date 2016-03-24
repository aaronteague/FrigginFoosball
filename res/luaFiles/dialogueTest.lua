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
dialogue = {normal = {}, special = {}, somthingelse = {}}

--create a function of dialogue
function talk()
 
 setemotion('mad', 30)
 say(0.6, 30, false, 'Wow, I really like talking to ' .. getPlayerName())


 say(0.5, 20, false, 'If i could, i would talk all the time, to people like ' .. getNeighborName())
 
 say(0.8, 30, false, 'I literally would never shut up, litterly, i would like a nice ' .. getItemName())
 
 popupquestion('yes', 'no', 'maybe', 'shut the fuck up')
 say(0.4, 30, true, 'I would talk for hours and hours')
 
 
 if arg == 'yes' then
  say(0.4, 30, false, 'I agree')
 elseif arg == 'no' then
  say(0.4, 30, false, 'well, you are wrong')
 elseif arg == 'maybe' then
  say(0.4, 30, false, 'yes.  just maybe, or maybe no')
 elseif arg == 'shut the fuck up' then
  say(0.4, 30, false, 'wow.  you are a really rude person arent you?')
  say(0.4, 30, false, 'I would never talk like that to you')
 else
  error("not an answer")
 end
 
 finish()
end

--store the dialogue function into a table
dialogue['normal']['derp'] = talk

dialogueTable = {Pamela = dialogue}