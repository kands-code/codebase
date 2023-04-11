module Main (main) where

import Test.Hspec
  ( describe,
    hspec,
    it,
    shouldBe,
  )
import Tool (add)

main :: IO ()
main = hspec $ do
  describe "Prelude.head" $ do
    it "4 add 9 should be 13" $ do
      (4 `add` 9) `shouldBe` 13