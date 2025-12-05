SAMPLE_TEST_INPUT_DIR=./sample/input
SAMPLE_TEST_EXPECTED_DIR=./sample/expected
SAMPLE_TEST_OUTPUT_DIR=./sample/output

BUILD_DIR=./outDebug
HEAP_APP=$BUILD_DIR/heap-app
CSP_APP=$BUILD_DIR/csp-app

if [[ -d $SAMPLE_TEST_OUTPUT_DIR ]]
then
    rm -rf $SAMPLE_TEST_OUTPUT_DIR
fi

mkdir -p $SAMPLE_TEST_OUTPUT_DIR

NUM_HEAP_TESTS=$(ls $SAMPLE_TEST_INPUT_DIR/heap_*.txt | wc -l)
NUM_CSP_TESTS=$(ls $SAMPLE_TEST_INPUT_DIR/csp_*.txt | wc -l)

NUM_PASSED_HEAP_TESTS=0
if [ -f $HEAP_APP ]; then
  for test in $SAMPLE_TEST_INPUT_DIR/heap_*.txt; do
    TESTNAME=$(basename $test)
    $HEAP_APP $SAMPLE_TEST_INPUT_DIR/$TESTNAME > $SAMPLE_TEST_OUTPUT_DIR/$TESTNAME
  
    # Get differences between outputs
    diff -ZB $SAMPLE_TEST_OUTPUT_DIR/$TESTNAME $SAMPLE_TEST_EXPECTED_DIR/$TESTNAME >/dev/null 2>&1
    status=$?
    if [ "$status" == "0" ]; then
      NUM_PASSED_HEAP_TESTS=$(( NUM_PASSED_HEAP_TESTS + 1 ))
      echo "Passed test $TESTNAME"
    else
      echo "Failed test $TESTNAME"
    fi
  done
fi

NUM_PASSED_CSP_TESTS=0
if [ -f $CSP_APP ]; then
  for test in $SAMPLE_TEST_INPUT_DIR/csp_*.txt; do
    TESTNAME=$(basename $test)
    $CSP_APP < $SAMPLE_TEST_INPUT_DIR/$TESTNAME > $SAMPLE_TEST_OUTPUT_DIR/$TESTNAME
  
    # Get differences between outputs
    /voc/private/validate-output-app $SAMPLE_TEST_INPUT_DIR/$TESTNAME $SAMPLE_TEST_EXPECTED_DIR/$TESTNAME $SAMPLE_TEST_OUTPUT_DIR/$TESTNAME >/dev/null 2>&1
    status=$?
    if [ "$status" == "0" ]; then
      NUM_PASSED_CSP_TESTS=$(( NUM_PASSED_CSP_TESTS + 1 ))
      echo "Passed test $TESTNAME"
    else
      echo "Failed test $TESTNAME"
    fi
  done
fi

echo
echo ==================================================
echo "Part 1: "
echo "Passed $NUM_PASSED_HEAP_TESTS / $NUM_HEAP_TESTS tests"
echo ==================================================
echo "Part 2: "
echo "Passed $NUM_PASSED_CSP_TESTS / $NUM_CSP_TESTS tests"
echo ==================================================
echo