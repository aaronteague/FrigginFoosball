 function stage1()

 gameTable:Load("res/table.lua")
 opponent:Load("res/Aylott/opponent.lua")
 player:Load("res/Pamela/player.lua")
   
 loadAnnouncerSounds()
 GAME:PlayMusic("res/music/Hep Cats.ogg")
 
 setEventOnScore(opponent, 1, opponentDance)
 setEventOnWin(1, playerWin)
 setEventOnWin(2, opponentWin)
 
 opponent:SetPosition(opponentStartPos)
 player:SetPosition(playerPlayPos)
 
 camera:SetPosition(tableCamOffset)
 camera:LookAt(gameTable)
 
 announcer:Say(0)
 wait(4000)

 camera:SetPosition(opponentCamOffset)
 camera:Follow(opponent)
 
 opponent:WalkToPosition(opponentPlayPos, 1)
 pause()  -- wait to get to position
 
 opponent:setAnimState("Come_In")
 wait(4000) 
 
 camera:SetPosition(playerCamOffset)
 camera:LookAt(player)
 player:setAnimState("Come_In")
 wait(3000)
 
 
 camera:SetState("Round_Ready")
 GAME:SwoopLogo(2000)
 pause()
 GAME:StartRound()
 
 -- rest is handled from callbacks
end

function loadAnnouncerSounds()
 announcer:LoadFile(0, "res/Stage1/stage1intro.ogg")
 announcer:LoadFile(1, "res/Stage1/player_win.ogg")
 announcer:LoadFile(2, "res/Stage1/player_lose.ogg")
end

function opponentDance()
 -- go back here please
end

function playerWin()
 announcer:Say(1)
 --wait(3000)
end

function opponentWin()
 announcer:Say(2)
 --wait(3000)
end



----------------

co = coroutine.create(stage1)
coroutine.resume(co)