module Main (main) where

import           Test.Hspec (describe, hspec, it, shouldBe)
import           Tool       (fib)

main :: IO ()
main = hspec $ do
  describe "Prelude.head" $ do
    it "fib 7 should be 8" $ do
      fib 7 `shouldBe` 8
