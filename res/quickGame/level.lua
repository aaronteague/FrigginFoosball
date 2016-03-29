  function quickGame()

 -- load up visual assets
 
 level = GAME:StartGame()
 
 gameTable:Load("res/table.lua")
 opponent:Load("res/Aylott/opponent.lua")
 GAME:PlayMusic("Blown Away - No Percussion")
 --player:Load("res/Pamela/player.lua")
 setEventOnScore(player, 1, playerScore1)
 -- load up the sounds
 
 --loadAnnouncerSounds()
 
 --announcer:Say(0) -- stage 1 intro
 
 -- set up the events
 --setEventOnScore(opponent, 1, opponentScore1)
 --setEventOnScore(player, 1, playerScore1)
 ---setEventOnWin(1, playerWin)
 --setEventOnWin(2, opponentWin)
 --setEventOnLongRunTime(longTimeFunc)
 --setEventOnPowerUpReceived
 
 --camera:SetPosition(tableCamOffset)
 --camera:PanAround(gameTable, -3, 10)
 
 --WaitForTime(3500)
 --opponent:SetPosition(opponentStartPos)
 --opponent:FadeToCharacter()
 --opponent:WalkToPosition(opponentPlayPos)
 --camera:SetPosition(opponentStartCamOffset)
--camera:Follow(opponent)
 
 --WaitForTime(6000)
 --player:SetPosition(playerStartPos);
 --player:ShowCharacter()
 --player:WalkToPosition(playerPlayPos);
 --camera:SetPosition(playerStartCamOffset)
 --camera:Follow(player)
 --WaitForTime(8000)
 
 --player:setAnimState("Wave")
 --WaitForTime(8000)
 

 
 
  camera:SetState("Round_Ready")
 --level:SwoopLogo(3000)
 
 --pause()
 level:StartRound()  -- make player invisible again
 
 -- rest is handled from callbacks
end

function loadAnnouncerSounds()
 --announcer:LoadFile(0, "res/Stage1/stage1intro.ogg")
 --announcer:LoadFile(1, "res/Stage1/player_win.ogg")
 --announcer:LoadFile(2, "res/Stage1/player_lose.ogg")
 --announcer:LoadFile(3, "res/Stage1/barrier.ogg")
 --announcer:LoadFile(4, "res/Stage1/long_time.ogg")
 --announcer:LoadFile(5, "res/Stage1/magnet.ogg")
 --announcer:LoadFile(6, "res/Stage1/pamela_scored_her_first.ogg")
 --announcer:LoadFile(7, "res/Stage1/round_1_goes_to_aylott.ogg")
 --announcer:LoadFile(8, "res/Stage1/speed.ogg")
 --announcer:LoadFile(9, "res/Stage1/speed_explained.ogg")
end

function opponentScore1()
 --announcer:Say(7) -- round 1 goes to aylott
 level:StartRound()
end

function playerScore1()
 ---------------- add this back right when it is time to release the game into the public
-- level:ShowAds()
 --pause()
 level:StartRound()
end

function playerWin()
 GAME:ResetTimer()
 announcer:Say(1)
 player:setAnimState("Victory")
 player:ShowCharacter()
 camera:SetState("Camera_Stage")
 camera:SetPosition(playerStartCamOffset)
 camera:LookAt(player)
 GAME:SaveProgress(1, 2)
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

function longTimeFunc()
 announcer:Say(4) -- long time
end



----------------

co = coroutine.create(quickGame)
coroutine.resume(co)