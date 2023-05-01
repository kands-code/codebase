module Tool (fib) where

fib :: Int -> Maybe Integer
fib n = fib' n 0 1
  where
    fib' m a b =
      if m < 1
        then Nothing
        else case m of
          1 -> Just a
          2 -> Just b
          _ -> fib' (m - 1) b (a + b)
