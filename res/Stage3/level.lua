 level = 0
 
 function stage1()
 level = GAME:StartGame()
 
 gameTable:Load("res/table.lua")
 opponent:Load("res/Aylott/opponent.lua")
 --player:Load("res/Pamela/player.lua")
   
   --wait(1000) -- test
 loadAnnouncerSounds()
 --GAME:PlayMusic("res/music/Hep Cats.ogg")
 
 setEventOnScore(opponent, 1, opponentDance)
 setEventOnBallCloseToGoal(saySomething)
 setEventOnPowerUpReceived(saySomething, "Speed", 1)
 setEventOnLongRunTime(saySomething)
 
 WaitForTime(1000)
 opponent:SetPosition(opponentStartPos)
 opponent:ShowCharacter()
 --player:SetPosition(playerPlayPos)
 
 --camera:SetPosition(tableCamOffset)
 --camera:LookAt(gameTable)
 
 --announcer:Say(0)
 --wait(4)

 --camera:SetPosition(opponentCamOffset)
 camera:PanAround(opponent, 10, 30)
 WaitForTime(1000)
 
 camera:ZoomIntoTarget(1000)
 
 WaitForTime(2000)
 --opponent:WalkToPosition(opponentPlayPos, 1)
 --pause()  -- wait to get to position
 
 --opponent:setAnimState("Come_In")
 --wait(4) 
 
 --camera:SetPosition(playerCamOffset)
 --camera:LookAt(player)
 --player:setAnimState("Come_In")
 --wait(3)
 
 
 
 --camera:SetState("Round_Ready")
 opponent:FadeToHands();
 level:SwoopLogo(200)
 pause()
 level:StartRound()
 
end

function loadAnnouncerSounds()
 announcer:LoadFile(0, "res/Stage3/intro.ogg")
 announcer:LoadFile(1, "res/Stage1/player_win.ogg")
 announcer:LoadFile(2, "res/Stage1/player_lose.ogg")
end

function opponentDance()
 -- go back here please
 GAME:ResetTimer()
 opponent:FadeToCharacter()
 camera:PeekUp(2000)
 WaitForTime(4000)
 opponent:FadeToHands()
 
 level:StartRound()
end

function playerWin()
 announcer:Say(1)
 --wait(3000)
end

function opponentWin()
 announcer:Say(2)
 --wait(3000)
end

function saySomething()
 announcer:Say(1)
end



----------------

co = coroutine.create(stage1)
coroutine.resume(co)