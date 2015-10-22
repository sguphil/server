
function runTest(Test, a)
    Test:testa()
end

function runTestb(Test)
    local ret = Test:testb(1000)
    print("=====ret:" .. ret)
end

