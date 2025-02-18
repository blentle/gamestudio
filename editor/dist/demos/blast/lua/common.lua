

function SpawnExplosion(pos, kind)
    local class = ClassLib:FindEntityClassByName(kind)
    local args  = game.EntityArgs:new()
    args.class  = class
    args.name   = 'explosion'
    args.position = pos
    Scene:SpawnEntity(args, true)
end

function SpawnScore(pos, score)
    local class = ClassLib:FindEntityClassByName('Score')
    local args  = game.EntityArgs:new()
    args.class  = class
    args.name   = 'score'
    args.position = pos
    args.logging  = false
    local entity = Scene:SpawnEntity(args, true)
    local node   = entity:FindNodeByClassName('text')
    local text   = node:GetTextItem()
    text:SetText(tostring(score))
end

function PlayMusic(music)
    if State.play_music then 
        Audio:PlayMusic(music)
    end
end