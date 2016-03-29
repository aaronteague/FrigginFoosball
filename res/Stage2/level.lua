  function stage2()

 -- load up visual assets
 level = GAME:StartGame()
 
 gameTable:Load("res/table.lua")
 opponent:Load("res/Aylott/opponent.lua")
 opponent:SetDifficulty(0.3)
 GAME:PlayMusic("3 am West End")
 player:Load("res/Pamela/player.lua")
 
 -- load up the sounds
 loadAnnouncerSounds()

 announcer:Say(0) -- stage intro
 
 -- set up the events
 setEventOnWin(1, playerWin)
 setEventOnWin(2, opponentWin)
 
 camera:SetPosition(tableCamOffset)
 camera:PanAround(gameTable, -3, 10)
 
 WaitForTime(3500)
 opponent:SetPosition(opponentStartPos)
 opponent:ShowCharacter()
 opponent:WalkToPosition(opponentPlayPos)
 camera:SetPosition(opponentStartCamOffset)
 camera:Follow(opponent)
 
 WaitForTime(6000)
 player:SetPosition(playerStartPos);
 player:ShowCharacter()
 player:WalkToPosition(playerPlayPos);
 camera:SetPosition(playerStartCamOffset)
 camera:Follow(player)
 
 WaitForTime(8000)

 level:SwoopLogo(3000)
 pause()
 level:StartRound()  -- make player invisible again
 
 -- rest is handled from callbacks
end

function loadAnnouncerSounds()
 announcer:LoadFile(0, "res/Stage2/intro.ogg")
 announcer:LoadFile(1, "res/Stage2/Pamela_Wins.ogg")
 announcer:LoadFile(2, "res/Stage2/Aylott_Wins.ogg")
end

function playerWin()
 GAME:ResetTimer()
 announcer:Say(1)
 player:setAnimState("Victory")
 player:ShowCharacter()
 camera:SetState("Camera_Stage")
 camera:SetPosition(playerStartCamOffset)
 camera:LookAt(player)
 GAME:SaveProgress(1, 3)
 WaitForTime(3000)
 GAME:ShowOverMap()
end

function opponentWin()
 GAME:ResetTimer()
 opponent:FadeToCharacter()
 WaitForTime(100)
 camera:SetState("Camera_Stage")
 announcer:Say(2)
 opponent:setAnimState("Victory")
 
 camera:SetPosition(opponentStartCamOffset)
 camera:LookAt(opponent)
 WaitForTime(3000)
 GAME:ShowOverMap()
end



----------------

co = coroutine.create(stage2)
coroutine.resume(co)