------------------------------------------------------------
------- binding functions.  just a little under the hood ---
------------------------------------------------------------

--lua

function poop()
 derp = 'suckit'
end

function setemotion(emotion, settoletter)
 character:setemotion(emotion, settoletter)
end


function popupquestion(one, two, three, four, five, six, seven)
 if one == nil then
  one = 'null'
 end
  if two == nil then
  two = 'null'
 end
  if three == nil then
  three = 'null'
 end
  if four == nil then
  four = 'null'
 end
  if five == nil then
  five = 'null'
 end 
 if six == nil then
  six = 'null'
 end 
 if seven == nil then
  seven = 'null'
  end

 character:popupquestion(one, two, three, four, five, six, seven)
 --coroutine.yield(answer)
end

function say(fontsize, textspeed, question, whattosay)
 character:say(fontsize, textspeed, question, whattosay)
 coroutine.yield()
end

function getPlayerName()
 return character:getPlayerName()
end

function getNeighborName()
 return character:getNeighborName()
end

function getItemName()
 return character:getItemName()
end

function setEmotion(emotion, letterToStart)
 character:setemotion(emotion, letterToStart)
end


--C++
function resume(id)
coroutine.resume(currentDialogue)
end



function answer(id, result)
 arg = result
 coroutine.resume(currentDialogue, result);
end

function finish()
 character:finish()
 currentDialogue = 0
end

function doDialogue(id, condition, category)
if currentDialogue == 0 then
 index = math.random(dialogueTable[id][condition][category]['size'])
 currentDialogue = coroutine.create(dialogueTable[id][condition][category][index])
 coroutine.resume(currentDialogue)
 end
end

function test()
  --say(0.6, 30, false, 'Wow, I really like talking to ')
end

-------characterTable-------------------------------------------------
currentDialogue = 0
dialogueTable = {}
