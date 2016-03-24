function loadAnnouncerSounds()
 announcer:LoadFile(0, "res/Stage1Sounds/stage1intro.ogg")
end

function opponentDance()
 -- go back here please
end

opponentStartPos = Vector3(2.1505 ,14.77164, -6.69674)
opponentPlayPos = Vector3(2.1505 ,9.76, -6.69674)
playerPlayPos = Vector3(-2.427 ,-10.092, -6.334)
tableCamOffset = Vector3(-20, -0.77, 15.1)
opponentCamOffset = Vector3(-17.983, 5.393, 13.749)
playerCamOffset = Vector3(-17.983, -9.168, 13.749)

playerScoreEvents = {}
opponentScoreEvents = {}

function stage1()

 gameTable:Load("res/table.lua")
 opponent:Load("res/Aylott/opponent.lua")
 player:Load("res/Pamela/player.lua")
 
 loadAnnouncerSounds()
 --GAME:PlayMusic("res/music/Vibe Ace.ogg")
 
 setEventOnScore(opponent, 1, opponentDance)
 
 opponent:SetPosition(opponentStartPos)
 player:SetPosition(playerPlayPos)
 
 camera:SetPosition(tableCamOffset)
 camera:LookAt(gameTable)
 
 --GAME:Start()

 --announcer:Say(0)
 wait(20)

 camera:SetPosition(opponentCamOffset)
 camera:Follow(opponent)
 
 opponent:WalkToPosition(opponentPlayPos, 1)
 pause()  -- wait to get to position
 
 opponent:setAnimState("Come_In")
 wait(1000) 
 
 camera:SetState("Round_Ready")
 GAME:SwoopLogo(4000)
 pause()
 GAME:StartRound()
 
 -- rest is handled from callbacks
end

-- lua called 

function wait(timeToWait)
GAME:Wait(timeToWait)
coroutine.yield()
end

function setEventOnScore(character, score, resultFunc)
  if character == player then
   playerScoreEvents[score] = resultFunc
  elseif character == opponent then
   opponentScoreEvents[score] = resultFunc
  else
   error("nope")
  end
end

------------------------

--C++ called

function resume()
coroutine.resume(co)
end

function pause()
coroutine.yield()
end

function notifyScore(characterID, score)
  func = nil
 if characterID == 1 then
   func = playerScoreEvents[score]
 elseif characterID == 2 then
   func = opponentScoreEvents[score]
 end
 
  if func ~= nil then
   func()
  end
  
  GAME:StartRound()
end

function notifyGameEnd(winningCharacter)

end



----------------

co = coroutine.create(stage1)
coroutine.resume(co)