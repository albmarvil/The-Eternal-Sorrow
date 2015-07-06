-- RNDID = 0

-- function nextRand()
-- 	-- body
-- 	local id = RNDID + 1
-- 	RNDID = id
-- 	return id
-- end

function RandomBasic()
	local rnd = RandomUtils.Random()
	return rnd
end

function Random(n)
	local rnd = RandomUtils.RandomRange(0,n)
	return rnd

end

function RandomRange(min, max)
	local rnd = RandomUtils.RandomRange(min, max)
	return rnd
end

function RandomElement( tabla )
	local element = RandomUtils.RandomRange(1, table.getn(tabla))
	return element
end


function setRandomSeed(seed)
	RandomUtils.setSeed(seed)
end