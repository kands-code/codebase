module Main where

import Tool qualified (add)

main :: IO ()
main = do
  print $ Tool.add 2 3
