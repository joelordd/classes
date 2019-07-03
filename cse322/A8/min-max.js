class MinMax {
    constructor(numbers) {
        this.numbers = numbers;
    }
    
    findMinMax() {
        let min = this.numbers[0];
        let max = this.numbers[0];
    
        for(let i = 0; i < this.numbers.length; i++) {
            if(min > this.numbers[i]) { min = this.numbers[i];}
            if(max < this.numbers[i]) { max = this.numbers[i];}
        }
        
        return `[${min}, ${max}]`;
    }
    
    removeMax() {
        let max = this.numbers[0];
    
        for(let i = 0; i < this.numbers.length; i++) {
            if(max < this.numbers[i]) { max = this.numbers[i];}
        }
        
        for(let i = 0; i < this.numbers.length; i++) {
            if(max === this.numbers[i]) {
                this.numbers.splice(i, 1);
                break;
            }
        }
    }
}
        const num = new MinMax([12, 34, 5, 76, 4, 76]);
        
        console.log(num.findMinMax());
        num.removeMax();
        console.log(num.numbers);