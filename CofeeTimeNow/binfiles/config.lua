-- [Base options]
--------------------------------------------------------------

-- Через какое время делать перерыв (секунды)
breakEverySec = 25 * 60  -- 25 min

-- Продолжительность перерыва (секунды)
breakDurationSec = 30 

-- Через сколько времени отсутствия действий от пользователя начнется IDLE (секунды)
idleTimeAfterSec = 100

-- Добавить в автозагрузку
addToAutoStart = 0

-- Отключить в программах. example: "game1.exe,game2.exe"
disableInPrograms = ""



-- [LockWnd options]
--------------------------------------------------------------

-- Автоматически закрывать блок-окно после окончания перерыва
autoCloseLockWnd = 1

-- Блокирующее окно можно закрыть правой кнопкой мыши 
closeLockWndByRightMouseBtn = 1

-- Время анимации блокирующего окна (миллисекунды)
lockWndAnimateTimeMs = 700

-- Тип анимации появления блокирующего окна
-- 0 - без анимации
-- 1 - fade анимация
-- 2 - from top to bottom
lockWndAnimateType = 1

-- FONT

-- "Verdana"
-- "Tahoma" 
-- "FixedSys"
lockWndFontFamily = "verdana" 

lockWndFontSize = 36

lockWndFontColor = 0xFFFFFF

-- 0 - default, 400 - normal, 700 - bold, 900 - heavy
lockWndFontWeight = 0 

-- 0x000000 - black
-- 0x1f3447 - dark blue
-- 0x81314c - dark red
-- 0x235c23 - dark green
-- 0x4d2277 - dark purple
lockWndBackgroundColor = 0x1f3447


-- [Debug options]
--------------------------------------------------------------

-- Уроверь логирования. 0 - ничего, 4 - максимальный
ll=0

-- Размер блокировочного окна (пиксели). 0 - fullscreen
lockWndSize = 0

-- Минимальное время простоя, до сброса таймера перерыва (миллисекунды)
minIdleRestartWaitMs = 5 * 60 * 1000



-- [Not supported or untested options]
--------------------------------------------------------------



