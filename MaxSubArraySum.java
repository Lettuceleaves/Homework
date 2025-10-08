public class MaxSubArraySum {
    public static int findMaxSubArraySum(int[] arr) {
        if (arr == null || arr.length == 0) {
            return 0;
        }
        int max_so_far = arr[0];
        int max_ending_here = arr[0];

        for (int i = 1; i < arr.length; i++) {
            max_ending_here = Math.max(arr[i], max_ending_here + arr[i]);
            max_so_far = Math.max(max_so_far, max_ending_here);
        }

        return max_so_far;
    }

    public static void main(String[] args) {
        int[] arr1 = {1, -2, 3, 5, -1};
        int[] arr2 = {1, -2, 3, -8, 5, 1};
        int[] arr3 = {1, -2, 3, -2, 5, 1};
        int[] arr4 = {-2, -3, -1, -5};

        System.out.println(findMaxSubArraySum(arr1));
        System.out.println(findMaxSubArraySum(arr2));
        System.out.println(findMaxSubArraySum(arr3));
        System.out.println(findMaxSubArraySum(arr4));
    }
}