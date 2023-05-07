module Main where

import Text.Printf (printf)
import Tool (fib, fromMaybe)

main :: IO ()
main = do
  let fib1k = fromMaybe (-1) $ fib 1000
  printf "real: %d\n" fib1k
  putStrLn "============================================"
  printf "log: %f\n" (log $ fromIntegral fib1k :: Double)
