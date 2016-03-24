opponentStartPos = Vector3(2.1505 ,15.77164, -6.69674)
opponentPlayPos = Vector3(2.1505 ,9.76, -6.69674)
playerPlayPos = Vector3(-2.427 ,-10.092, -6.334)
tableCamOffset = Vector3(-20, -0.77, 15.1)
opponentCamOffset = Vector3(-17.983, 7.393, 13.749)
playerCamOffset = Vector3(-0.983, 10.168, 9.749)


playerScoreEvents = {}
opponentScoreEvents = {}

 p1WinFunc = nil
 p2WinFunc = nil

poleBounds = {}
poleBounds[1] = 3.1
poleBounds[2] = 2.7
poleBounds[3] = 3.1
poleBounds[4] = 3.3
poleBounds[5] = 3.3
poleBounds[6] = 3.1
poleBounds[7] = 2.7
poleBounds[8] = 3.1

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
-- if notifyCo ~= 0 then
 -- coroutine.resume(notifyCo)
 -- co = notifyCo
-- end
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
   --coroutine.yield()
   co = coroutine.create(func) -- resume on update
   coroutine.resume(co)
  else
   level:StartRound()
  end
 
end

 function notifyGameEnd(winningCharacter)
  if winningCharacter == 1 then
   p1WinFunc()
  elseif winningCharacter == 2 then
   p2WinFunc()
  end
 GAME:SaveProgress(1, 1)
  GAME:ShowOverMap()
 end
 
 function notifyLongTime()
  func = nil
  index = math.random(#longRunTimeFuncs)
  func = longRunTimeFuncs[index]
  
  if func ~= nil then 
   co = coroutine.create(func)
   coroutine.resume(co)
  end
  
 end
 
 function notifyNearGoal(goalIndex)
  func = nil
  index = math.random(#closeToGoalFuncs)
  func = closeToGoalFuncs[index]
  
  if func ~= nil then
   co = coroutine.create(func)
   coroutine.resume(co)
  end
   
 end
 
 function notifyPowerup(powerup, playerIndex)
  func = nil
  func = powerUpReceivedFuncs[powerup][playerIndex]
  
  if func ~= nil then
   co = coroutine.create(func)
   coroutine.resume(co)
  end
  
 end

overPos = {}
overPos[1] = {535, 1599, "res/Stage1/level.lua"}
overPos[2] = {1164, 1495, "res/Stage2/level.lua"}
overPos[3] = {1435, 1429, "res/Stage3/level.lua"}
overPos[4] = {1443, 1255, "res/Stage4/level.lua"}
overPos[5] = {1197, 1204, "res/Stage5/level.lua"}
overPos[6] = {1038, 1084, "res/Stage6/level.lua"}
overPos[7] = {843, 978, "res/Stage7/level.lua"}



-- lua called 

function wait(timeToWait)
GAME:Wait(timeToWait)
coroutine.yield()
end

function WaitForTime(eventTime)
GAME:WaitForTime(eventTime)
--wakeUpTime = eventTime
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



 function setEventOnWin(characterInt, winFunc)
  if characterInt == 1 then
   p1WinFunc = winFunc
  elseif characterInt == 2 then
   p2WinFunc = winFunc
  end
 end
 
 closeToGoalFuncs = {}
 
 powerUpReceivedFuncs = {Barrier = {}, Speed = {}, Magnet = {}}
 
 longRunTimeFuncs = {}
 
 function setEventOnBallCloseToGoal(func)
  table.insert(closeToGoalFuncs, func)
 end
 
 function setEventOnPowerUpReceived(func, power, characterID)
  table.insert(powerUpReceivedFuncs[power], characterID, func) -- hope this works
 end
 
 function setEventOnLongRunTime(func)
  table.insert(longRunTimeFuncs, func)
 end

------------------------

--C++ called

function resume()
coroutine.resume(co)

end

function pause()
coroutine.yield()
end





function start()
 --GAME:PlayMusic("res/music/Vibe Ace.ogg")
 logoScreen = GAME:ShowLogo()

 WaitForTime(100)

 logoScreen:ShowClickToStart()

 pause()
 --GAME:PlaySound("some file")
 GAME:ShowOverMap()
 --dofile("res/Stage3/level.lua")
end

----------------

co = coroutine.create(start)
coroutine.resume(co)